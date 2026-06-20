import numpy as np


def my_exam(x):
    f = x[0] ** 2 + 2 * x[1] ** 2
    g = np.array([2 * x[0], 4 * x[1]])
    return f, g


def steep_descent(f_name, x0, tstep, eps,decay_rate):
    xk = np.array(x0, dtype=float)
    iter_count = 0
    max_iter = 10000

    while True:
        fk, gk = f_name(xk)
        if np.linalg.norm(gk) < eps:
            break

        xk = xk - tstep * gk
        iter_count += 1
        if iter_count >= max_iter:
            print("达到最大迭代次数")
            break

        tstep=tstep*decay_rate

    xstar = xk
    fxstar, _ = f_name(xstar)
    return xstar, fxstar, iter_count


if __name__ == '__main__':
    x0 = [1.0, 1.0]
    tstep =0.1
    eps = 1e-3
    decay_rate = 0.99

    xstar, fxstar, iters = steep_descent(my_exam, x0, tstep, eps,decay_rate)

    print(f"最优解 xstar: {xstar}")
    print(f"最优函数值 fxstar: {fxstar:.8f}")
    print(f"迭代次数: {iters}")