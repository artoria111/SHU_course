import numpy as np
from numpy import isclose
from numpy.typing import NDArray

def get_frac(x):
    return x - np.floor(x)

def pivot_operation(matrix, b_vec, r, c):
    pivot_val = matrix[r, c]
    matrix[r] = matrix[r] / pivot_val
    b_vec[r] = b_vec[r] / pivot_val

    rows, cols = matrix.shape

    for i in range(rows):
        if i != r:
            factor = matrix[i, c]
            matrix[i] = matrix[i] - factor * matrix[r]
            b_vec[i] = b_vec[i] - factor * b_vec[r]

#返回最优单纯形表和基变量索引
def simplex_eye(A, b, c):
    rows, cols = A.shape
    base_index = [0] * rows
    # 最优单纯形表
    success=False
    for i in range(cols):
        col = A[:, i]
        if np.count_nonzero(col) == 1 and isclose(np.sum(col), 1):
            row_idx = np.where(isclose(col, 1))[0][0]
            base_index[row_idx] = i

    iteration = 0
    while True:
        iteration += 1
        if iteration > 100:
            print("迭代已到上限")
            break
        #基变量对应的系数
        c_base = np.array([c[i] for i in base_index])
        #检验数
        delta = []
        for i in range(cols):
            col = A[:, i]
            delta_i = c[i] - np.dot(c_base, col)
            delta.append(delta_i)
        delta = np.array(delta)
        #检验数全部小于零，结束迭代
        if all(d <= 0 for d in delta):
            success=True
            break
        #确定换入变量
        index_go_in = np.argmax(delta)
        col_go_in = A[:, index_go_in]
        theta = []

        for i in range(rows):
            if col_go_in[i] > 0:
                theta.append(b[i] / col_go_in[i])
            else:
                theta.append(float('inf'))
        if all(t == float('inf') for t in theta):
            print("该问题存在无界解")
            return None, None, iteration
        #确定换出变量
        index_go_out = np.argmin(theta)
        base_index[index_go_out] = index_go_in
        pivot_operation(A, b, index_go_out, index_go_in)

    return success,A,b,base_index

def Dsimplex_eye(A, b, c):
    rows, cols = A.shape
    #base_index[i]代表单纯形表的第i行的基变量
    base_index = [0] * rows
    #确定初始基变量
    success=False
    for i in range(cols):
        col = A[:, i]
        if np.count_nonzero(col) == 1 and isclose(np.sum(col), 1):
            row_idx = np.where(isclose(col, 1))[0][0]
            base_index[row_idx] = i

    #迭代次数
    iteration = 0
    while True:
        iteration += 1
        if iteration > 100:
            print("迭代已到上限")
            break
        #基变量对应的系数
        c_base = np.array([c[i] for i in base_index])
        #检验数
        delta = []
        for i in range(cols):
            col = A[:, i]
            delta_i = c[i] - np.dot(c_base, col)
            delta.append(delta_i)
        delta = np.array(delta)
        #所有b值全部大于等于零，结束迭代
        if all(val >= 0 for val in b):
            success=True
            break
        #确定换入变量
        index_go_out = np.argmin(b)
        col_go_in = A[index_go_out, :]
        if all(val>=0 for val in col_go_in):
            return None,None,iteration

        theta = []
        for i in range(cols):
            if col_go_in[i] < 0:
                theta.append(delta[i] / col_go_in[i])
            else:
                theta.append(float('inf'))
        #确定换出变量
        index_go_in = np.argmin(theta)
        base_index[index_go_out] = index_go_in
        pivot_operation(A, b, index_go_out, index_go_in)


    return success,A,b,base_index


def gomory(A,b,c)->[NDArray, float, float]:
    A = A.astype(float)
    b = b.astype(float)
    c = c.astype(float)
    rows, cols = A.shape
    iteration = 0
    success, best_A,best_b, basis_indices = simplex_eye(A, b, c)
    while success:
        iteration += 1
        #检测是否都为整数
        is_integer = True
        target_row_idx = -1
        max_frac = 0
        for i in range(len(b)):
            val=best_b[i]
            frac=get_frac(val)
            if abs(round(val) - val) >= 1e-3:
                is_integer = False
                #选择小数部分最大的行来构造割平面
                if frac > max_frac:
                    max_frac = frac
                    target_row_idx = i

        if is_integer:
            break

        cut_row=-get_frac(best_A[target_row_idx,:])
        rows=rows+1
        cols=cols+1
        new_A=np.zeros((rows,cols))
        new_A[0:rows-1,0:cols-1]=best_A
        new_A[rows-1,0:cols-1]=cut_row
        new_A[0:rows-1,cols-1]=0
        new_A[rows-1,cols-1]=1.0

        new_b=np.zeros(rows)
        new_b[0:rows-1]=best_b
        new_b[rows-1]=-get_frac(best_b[target_row_idx])
        basis_indices.append(cols)
        c=np.append(c,0)

        success,best_A,best_b,basis_indices = Dsimplex_eye(new_A,new_b,c)

    x_star = np.zeros(cols)
    for r in range(rows):
        col_idx = basis_indices[r]
        x_star[col_idx] = best_b[r]
    fx_star = np.dot(x_star, c)
    return x_star, fx_star, iteration



if __name__ == '__main__':
    #常规问题
    print("\n常规问题求解中......")
    A = np.array([
        [-1, 3, 1, 0],
        [7,1,0,1]
    ], dtype=float)
    b = np.array([6, 35], dtype=float)
    c = np.array([7,9,0,0], dtype=float)
    x_star, fx_star, iter_count = gomory(A, b, c)
    print("最优解 X:", x_star)
    print("目标函数值 Z:", fx_star)
    print("迭代次数:", iter_count)