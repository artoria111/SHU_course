import numpy as np
from numpy import isclose
from numpy.typing import NDArray


def Dsimplex_eye(A, b, c) -> [NDArray, float, float]:
    #浮点型强制转换
    A = A.astype(float)
    b = b.astype(float)
    c = c.astype(float)
    rows, cols = A.shape
    #base_index[i]代表单纯形表的第i行的基变量
    base_index = [0] * rows
    #确定初始基变量
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

    x_opt = np.zeros(cols)
    for r in range(rows):
        col_idx = base_index[r]
        x_opt[col_idx] = b[r]
    fx_opt = np.dot(x_opt, c)

    return x_opt, fx_opt, iteration




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


if __name__ == '__main__':
    #常规问题
    print("\n常规问题求解中......")
    A = np.array([
        [-1, -2, -1, 1, 0],
        [-2,1,-3,0,1]
    ], dtype=float)
    b = np.array([-3, -4], dtype=float)
    c = np.array([-2, -3, -4, 0,0], dtype=float)
    x_opts, fx_opt, iter_count = Dsimplex_eye(A, b, c)
    print("最优解 X:", x_opts)
    print("目标函数值 Z:", fx_opt)
    print("迭代次数:", iter_count)
