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

