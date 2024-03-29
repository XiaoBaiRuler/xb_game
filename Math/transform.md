## 1. 定理

**1. 线性变换$\tau$**
$$
设\tau(v) = \tau(x, y, z) = (x^1, y^1, z^1) \tau(u) 符合线性变换 \\
当且仅当: \tau(u + v) = \tau(u) + \tau(v) 且\tau(ku) = k\tau(u) k\in 标量
\tag{1.1}
$$

$$
u = (x, y, z) 可以写作: \\
u = x(1, 0, 0) + y(0, 1, 0) + z(0, 0, 1) = xi + yj + zk \\
其中i,j,k表示3D坐标向量的集合的标准基向量\tag{1.2}
$$

**2. 旋转**

![image-20220925201150695](./picture\vector\image-20220925201150695.png)
$$
求R_n(v), \\
已知v = (x, y, z), \theta, \alpha: \\
=> proj_n(v) = n||v|| * cos(\alpha) \\
=> v - proj_n(v) = v - n||v||cos(\alpha) \\
=> R_n(v) = proj_n(v) + k(?)
$$
![image-20220925202817364](./picture\vector\image-20220925202817364.png)
$$
a = (v - proj(v)) cos(\theta) \\
b = (n \times v) sin(\theta) \\
=> k = a + b \\
=> R_n(v) = proj_n(v) + cos(\theta)(v - proj(v)) + sin(\theta)(n \times v) \\
	= (n · v) n + cos\theta(v - (n · v)n) + sin\theta(n \times v) \\
	= cos\theta v + (1 - cos\theta)(n · v)n + sin\theta(n \times v)
$$

**3. 仿射变换部分概念**

```mathematica
1. 仿射变换: 线性变换 + 平移变换 
2. 齐次坐标:
	- 向量: (x, y, z, 0)
	- 点:  (x, y, z, 1)
	=> 两个点只差为向量，点与向量之和为点

3. 仿射变换矩阵表示(线性变换 + 平移向量):
	a(u) = t(u) + b
```

**4. 平移变换**
$$
平移: 不改变其方向和大小, 只改变点的位置(每个点沿着某个向量进行移动) \\
对应仿射变换: \tau(u) = uI + b: \\
=> \tau的矩阵表示(平移矩阵): \\
\left[\begin{matrix}
1 & 0 & 0 & 0 \\
0 & 1 & 0 & 0 \\
0 & 0 & 1 & 0 \\
b_z & b_y & b_z & 1 \\
\end{matrix}\right] \\
=> 逆矩阵:\\
\left[\begin{matrix}
1 & 0 & 0 & 0 \\
0 & 1 & 0 & 0 \\
0 & 0 & 1 & 0 \\
-b_z & -b_y & -b_z & 1 \\
\end{matrix}\right] \tag{1.3}
$$

**5. 坐标变换**

- <mark style="color:green">不同标架间的坐标变换: 相对于标架A表示某量的标量k，转换为相对于标架B描述同一种量的新变量$k^{'}$(都是相对位置来说的)</mark>
  $$
  A: P = xu + yv + zw \\
  => B: P_B = xu_B + yv_B + zw_B \\
  
  => 包含平移: P_B = xu_B + yv_B + zw_B + Q_B (Q_B标架A中的原点) \\
  => 坐标变换矩阵(标架变换矩阵)
  \left[\begin{matrix}
  	x, y, z, w
  \end{matrix}\right]
  \left[\begin{matrix}
  	u_x & u_y & u_z & 0 \\
  	v_x & v_y & v_z & 0 \\
  	w_x & w_y & w_z & 0 \\
  	Q_x & Q_y & Q_z & 1 \\
  \end{matrix}\right] \tag{1.4}
  $$

- <mark style="color:blue;">坐标变换矩阵之间乘积满足结合律，但不满足交换律</mark>

- <mark style="color:red;">坐标交换矩阵是可逆的, 证明:</mark>
  $$
  假设给定向量P相对于标架B的坐标P_B, \\
  以及将坐标A转换到标架B的变换矩阵:  \\
  P_B = P_AM \space\space\space\space\space\space\space\space \\
  => P_BM^{-1} = P_AMM^{-1} \\
  => P_BM^{-1} = P_AI \space\space\space\space\space\space\space\space\space\space \\
  => P_BM^{-1} = P_A \space\space\space\space\space\space\space\space\space\space\space\space \tag{1.5}
  $$
  


## 2. 推理

$$
由(1.1)、(1.2)推导得到:\\
\tau(au + bv + cw) = a\tau(u) + b\tau(v) + c\tau(w) \tag{2.1}
$$

$$
由(1.1)、(1.2)、(2.1): \\
\tau(u) = \tau(xi, yj, zk) = x\tau(i) + y\tau(j) + z\tau(z)
= 
\left[\begin{matrix}
x, y, z
\end{matrix}\right]

\left[\begin{matrix}
 \tau(i) \\
 \tau(j) \\
 \tau(z) 
\end{matrix}\right] \\
= 
uA (矩阵A是线性变换\tau的矩阵表示法) \tag{2.2}
$$

<mark>证明缩放$S(x, y, z) = (S_xx, S_yy, S_zz)$是线性变换</mark>
$$
由(1.1)得到: \\
符合规律1: S(u + v) = (s_x(u_x + v_x), s_y(u_y + v_y), s_z(u_z + v_z)) \\
	= (s_xu_x, s_yu_y, s_zu_z) + (s_xv_x, s_yv_y, s_zv_z) \\
    = S(u) + S(v) \\
符合规律2: S(ku) = (s_xku_x, s_yku_y, s_zku_z) \\
	= k(s_xu_x, s_yu_y, s_zu_z) \\
	= kS(u) 
\tag{2.3}
$$


## 3. 应用

### 3.1 <mark>缩放: </mark>

**$S(x, y, z) = (S_xx, S_yy, S_zz)$,  相对于当前坐标系中的原点，令向量在x, y, z轴上分别以系数S_x, S_y, S_z进行缩放**
$$
由(1,2)、(2.2)、(2.3)得到:\\
S(x, y, z) = S(xi + yj + zk) = xS(i) + yS(j) + zS(k) \\
那么S的线性变换矩阵为:
\left[\begin{matrix}
 s_x & 0 & 0 \\
 0 & s_y & 0 \\
 0 & 0 & s_z 
\end{matrix}\right] \\
$$

### 3.2 <mark>旋转:</mark>

$$
c = cos\theta, s = sin\theta, v = (x, y, z) \\
R_n(v) = cos\theta v + (1 - cos\theta)(n · v)n + sin\theta(n \times v) \\
\left[\begin{matrix}
	c + (1 - c)x^2 & (1 - c)xy + sz & (1 - c)xz - sy \\
    (1 - c)xy - sz & c + (1 - c)y^2 & (1 - c)yz + sx \\
    (1 - c)xz + sy & (1 - c)yz - sx & c + (1 - c)z^2
\end{matrix}\right] \\
=> 由于各个行向量都是规范正交的，所以逆矩阵与逆置矩阵是一样的
$$



## 4. 补充

