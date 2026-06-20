import numpy as np
from numpy import isclose
from numpy.typing import NDArray


def simplex_eye(A, b, c) -> [NDArray, float, float]:
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
    #最优单纯形表中的检验数
    delta_prefect=[]
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
            delta_prefect=delta[:]
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

    x_opt = np.zeros(cols)
    for r in range(rows):
        col_idx = base_index[r]
        x_opt[col_idx] = b[r]
    x_opts=[x_opt]

    #开始检测是否存在多重解
    has_multi_solution = False
    alt_opt_idx=[]
    non_base_index=[i for i in range(cols) if i not in base_index]
    for i in non_base_index:
        if isclose(delta_prefect[i], 0):
            has_multi_solution = True
            alt_opt_idx.append(i)

    if has_multi_solution:
        go_in_col=alt_opt_idx[0]
        col_vec=A[:, go_in_col]
        theta=[]
        for i in range(rows):
            if col_vec[i] > 0:
                theta.append(b[i] / col_vec[i])
            else:
                theta.append(float('inf'))
        go_out_row=np.argmin(theta)
        base_index[go_out_row] = go_in_col
        pivot_operation(A, b, go_out_row, go_in_col)
        x_alt_opt = np.zeros(cols)
        for r in range(rows):
            col_idx = base_index[r]
            x_alt_opt[col_idx] = b[r]
        x_opts.append(x_alt_opt)
    fx_opt = np.dot(x_opt, c)
    return x_opts, fx_opt, iteration


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
        [2, 1, 1, 0],
        [1,3,0,1]
    ], dtype=float)
    b = np.array([40, 30], dtype=float)
    c = np.array([3, 4, 0, 0], dtype=float)
    x_opts, fx_opt, iter_count = simplex_eye(A, b, c)
    print("最优解 X:", x_opts)
    print("目标函数值 Z:", fx_opt)
    print("迭代次数:", iter_count)


    #无限解的问题
    print("\n无限解问题求解中......")
    A = np.array([
        [2, 7, 1, 0],
        [7,2,0,1]
    ], dtype=float)
    b = np.array([21, 21], dtype=float)
    c = np.array([4, 14, 0, 0], dtype=float)
    x_opts, fx_opt, iter_count = simplex_eye(A, b, c)
    print("无限解 X:", x_opts)
    print("目标函数值 Z:", fx_opt)
    print("迭代次数:", iter_count)

    #无界解的问题
    print("\n无界解问题求解中......")
    A = np.array([
        [1, -1, 1, 0],
        [-3,1,0,1]
    ], dtype=float)
    b = np.array([2, 4], dtype=float)
    c = np.array([2, 3, 0, 0], dtype=float)
    x_opts, fx_opt, iter_count = simplex_eye(A, b, c)
    if x_opts and fx_opt:
        print("最优解 X:", x_opts)
        print("目标函数值 Z:", fx_opt)
        print("迭代次数:", iter_count)