## 1. 定理

**1. m $\times$ n 矩阵: ** m行n列的实数构成的矩阵阵列

- 元素以$M_{ij}$表示
- 行向量(x, y, z): $1 * n$的矩阵
- 列向量[x, y, z]: $m * 1$的矩阵

**2. 矩阵的一般运算**

- 矩阵相等: **当且仅当两个矩阵的对应元素相等**
- $m \times n$ 矩阵之间的加法: 两者对应元素相加
- 标量乘法: **标量依次和每个元素相乘**
- $m \times n$ 矩阵之间的减法: $A - B = A + (-1 \cdot B) = A + (-B)$

**3. 矩阵的乘法**	

​	**前提: 矩阵A:$m \times n$, 矩阵B: $n \times p$, 两者A的列数必须等于B的行数**

- $A \cdot B = A的行向量 \cdot B的的列向量 作为新的一个元素$

**4. 转置矩阵**

- **将原矩阵$M$的行与列进行互换得到新矩阵$M^T$**

**5. 单位矩阵**

- **主对角线上的元素均为1，其他为0的矩阵**
  $$
  \left[
  \begin{matrix}
  
  1 & \cdots & 0 \\
  \cdots & 1 & \cdots \\
  0 & \cdots & 1
  \end{matrix}
  \right]
  $$
  

- **任何矩阵与单位矩阵相乘得到依然是原矩阵**

**6. 余子阵**

- **n $\times$ n的矩阵A，余子阵$\overline{A_{ij}}$ : 从A中去除第i行和第j列的(n - 1) $\times$ (n - 1) 矩阵**
  $$
  A = 
  \left[
  \begin{matrix}
  A_{11} & A_{12} & A_{13} \\
  A_{21} & A_{22} & A_{23} \\
  A_{31} & A_{32} & A_{33}
  \end{matrix}
  \right]
  
  \\ \Longrightarrow \overline{A_{ij}} = 
  \left[
  \begin{matrix}
  A_{22} & A_{23} \\
  A_{32} & A_{33}
  \end{matrix}
  \right]
  $$

**7. 行列式**

- 在线性变换下，n(n > 2)维多面体体积变化的相关信息， 在二维下反映的是平行四边形有向面积的变化.

- 对于矩阵A(n $\times$ n)矩阵(n > 1):
  $$
  det A = \sum_{j=1}^n {A_{1j}(-1)^{1 + j}det\overline{A_{1j}}}
  \\ \\ eg: 四维:
  A = 
  \left [
  \begin{matrix}
  A_{11} & A_{12} & A_{13} & A_{14} \\
  A_{21} & A_{22} & A_{23} & A_{24} \\
  A_{31} & A_{32} & A_{33} & A_{34} \\
  A_{41} & A_{42} & A_{43} & A_{44} \\
  \end{matrix}
  \right]
  
  \\ = A_{11}
  
  \left [
  \begin{matrix}
  A_{22} & A_{23} & A_{24} \\
  A_{32} & A_{33} & A_{34} \\
  A_{42} & A_{43} & A_{44} \\
  \end{matrix}
  \right]
  
  - A_{12}
  
  \left [
  \begin{matrix}
  A_{21} & A_{23} & A_{24} \\
  A_{31} & A_{33} & A_{34} \\
  A_{41} & A_{43} & A_{44} \\
  \end{matrix}
  \right]
  
  \\ + A_{13}
  
  \left [
  \begin{matrix}
  A_{21} & A_{22} & A_{24} \\
  A_{31} & A_{32} & A_{34} \\
  A_{41} & A_{42} & A_{44} \\
  \end{matrix}
  \right]
  
  - A_{14}
  
  \left [
  \begin{matrix}
  A_{21} & A_{22} & A_{23} \\
  A_{31} & A_{32} & A_{33} \\
  A_{41} & A_{42} & A_{43} \\
  \end{matrix}
  \right]
  
  \\ \cdots
  $$

**8. 伴随矩阵**

- **对于矩阵A(n $\times$ n)矩阵(n > 1): $C_{ij} = (-1)^(i + j)det\overline{A_{ij}}$  其中$C_{i, j}作为矩阵C_A中的i行j列的元素，那么获得矩阵A的代数余子式矩阵C_A$ **
- $C_A$的转置矩阵$C_A^T$ 就是矩阵A的伴随矩阵$A^*$

**9. 逆矩阵**
$$
1. 只有方阵才有逆矩阵 \\
2. 矩阵M(n \times n)的逆矩阵M^{-1}(n \times n) \\
3. 可逆矩阵:存在逆矩阵的方阵; 奇异矩阵: 不存在逆矩阵的方阵\\
4. 可逆矩阵的逆矩阵是唯一的 \\
5. 矩阵与其逆矩阵相乘得到单位方阵: \\
	MM^{-1} = M^{-1}M = I \\
6. A^{-1} = \frac{A^*}{detA}
$$




## 2. 推理

**1. 矩阵加法性质**

- 加法交换律: $A + B = B + A$
- 加法结合律: $(A + B) + C = A + (B + C)$
- 标量乘法对矩阵加法的分配律: $r(A + B) = rA + rB$
- 矩阵乘法对标量加法的分配律: $(r + s)A = rA + sA$

**2. 矩阵减法性质**

- **2.4. $A - B = A + (-B)$, 那么减法也有加法相同的性质** 

**3. 矩阵之间乘法性质**

- 一般矩阵的乘法不满足**交换律**: 由矩阵乘法的前者列数必须等于后者的行数，所以

- 向量与矩阵的乘法 =  **向量u给定的标量系数x, y, z与矩阵A中各行向量的线性组合:**

  ---

  $$
  u \cdot A
  
  = 
  
  \left[
  \begin{matrix}
  	x & y & z
  \end{matrix}\right]
  
  \cdot
  
  \left[
  \begin{matrix}
  A_{11} & A_{12} & A_{13} \\
  A_{21} & A_{22} & A_{23} \\
  A_{31} & A_{32} & A_{33}
  \end{matrix} \right]
  
  = 
  
  \left[
  \begin{matrix}
  	x & y & z
  \end{matrix}\right]
  
  \cdot
  
  \left[
  \begin{matrix}
  \uparrow & \uparrow & \uparrow \\
  A_{*1} & A_{*2} & A_{*3} \\
  \downarrow & \downarrow & \downarrow
  \end{matrix} \right]
  
  \\\\
  = 
  [
  xA_{11} + yA_{21} + zA_{31}, 
  xA_{12} + yA_{22} + zA_{32},
  xA_{13} + yA_{23} + zA_{33}, 
  ]
  \\
  = 
  [xA_{11}, xA_{12}, xA_{13}]
  +
  [yA_{21}, yA_{22}, yA_{23}]
  +
  [zA_{31}, zA_{32}, zA_{33}]
  \\
  = x[A_{11}, A_{12}, A_{13}] + y[A_{21}, A_{22}, A_{23}] + z[A_{31}, A_{32}, A_{33}]
  \\
  = xA_{1*} + yA_{2*} + zA_{3*}
  $$

  ---

- 乘法之间的结合律

  ---

  $$
  A(B + C) = AB + AC
  \\
  (A + B)C = AC + BC
  $$

  ---


**4. 转置矩阵的性质**
$$
1. (A + B)^T = A^T + B^T \\
2. (CA)^T = cA^T........ \\
3. (A^T)^T = A........... \\
? 4. (A^{-1})^T = (A^T)^{-1}..... \\ \\
5. (AB)^T = (
\left[
\begin{matrix}
A_{11} & \cdots & A_{1m} \\
\cdots & \cdots & \cdots \\
A_{n1} & \cdots & A_{nm}
\end{matrix} \right]

\cdot

\left[
\begin{matrix}
B_{11} & \cdots & B_{1m} \\
\cdots & \cdots & \cdots \\
B_{n1} & \cdots & B_{nm}
\end{matrix} \right])^T

 = (

\left[
\begin{matrix}
\sum A_{1*} \times B_{*1} & \cdots & \sum A_{1*} \times B_{*m} \\
\cdots & \cdots & \cdots \\
\sum A_{n*} \times B_{*1} & \cdots & \sum A_{n*} \times B_{*m}
\end{matrix} \right])^T

\\ \\ =

\left[
\begin{matrix}
\sum A_{1*} \times B_{*1} & \cdots & \sum A_{n*} \times B_{*1} \\
\cdots & \cdots & \cdots \\
\sum A_{1*} \times B_{*m} & \cdots & \sum A_{n*} \times B_{*m}
\end{matrix} \right]

\\ \\ =

\left[
\begin{matrix}
B_{11} & \cdots & B_{n1} \\
\cdots & \cdots & \cdots \\
B_{1m} & \cdots & B_{nm}
\end{matrix} \right]

\cdot

\left[
\begin{matrix}
 A_{11} & \cdots & A_{n1} \\
 \cdots & \cdots & \cdots \\
 A_{1m} & \cdots & A_{nm}
\end{matrix} \right]

= B^T \cdot A^T
$$

---

**5. 行列式性质**

- **方阵A是可逆的，当且仅当$det A \neq 0$**

**6. 伴随矩阵的性质**
$$
AA^* = A^*A = |A|I = detAI
$$


**7. 逆矩阵的性质**
$$
同维度方阵下: 方阵之间乘法符合分配律\\
(AB)^{-1} = A^{-1}B^{-1}
$$


## 3. 应用

## 4. 补充

