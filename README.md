# Galois Field Hill Cipher


## Overview 

An implimentation of the Hill cipher extended by the use of Galois fields as described by Rodney Cooper in 1980.

Based on internet search and the article "An interesting example at the intersection
of Matrix Mathematics and Cryptography" by Tony Patti.

I strongly recommand reading the article, here is a link for the article in glassblower: [An interesting example at the intersection of Matrix Mathematics and Cryptography, by Tony Patti](https://www.glassblower.info/crypto/an-interesting-example-at-the-intersection-of-matrix-mathematics-and-cryptography-April-23-2024.pdf)

### The Original Hill Cipher

The Hill cipher is based on linear algebra.
It utilizes invertible matrixes to encrypt blocks of known length - it is a polygraphic substitution cipher.

##### The Encryption Process

Each vector of blocks(of size n) is multiplied with an invertible matrix(of size n x n). 
Afterwards, we calculate the result mod the maximum value for a block(2^n if the block is binary).

##### The Decyription Process

Each ciphertext vector is multiplied with the inverse of the encryption matrix. 
Afterwards, we calculate the result mod the maximum value for a block.

#### Security

##### KPA Vulnerable

The original Hill cipher is linear, which exposes it to KPA. With enought plaintext-ciphertext pairs.
An advesery can create a series of equations to find the encryption matrix.

##### Diffusion

As we showed in the previous part, a matrix multipication is vulnerable to KPA attacks.
But it allows us, when paired with a non-linear operation, to diffuse the ciphertexts - With a small change to the plaintext we would get a big change in the ciphertext when using matrix multipication.

##### Key Space Size

Let **n** be the matrix dimentation, and **K** be the number of possible block's values.
There are about K^(n^2) matrices - Including not usable matrices like non-inversible matrices.
Which results in best case scenerio key space of **Log2(K^(n^2))**

### Mathematical Background 

##### Galois Field

aka finite field. It's a field that contains a finite number of elements.

##### Matrix Multipication With Vector

Results in a matrix which each element in each row is multipied by the vector at the position of the coulmn.

*`*Example:**

[2 3 4]   [3]   [2*3 3*5 4*7]   [6 15 28 ]

[6 8 9] * [5] = [6*3 8*5 9*7] = [18 40 63]

[4 5 2]   [7]   [4*3 5*5 2*7]   [12 25 14]

### Implementation

##### Chosen Galois Field

The chosen field is GF(16,777,619)

###### Requirements

- Prime Number for defined modulo P, and exactly P number of elements.
- log2(P) < 32: That's for fitting the blocks inside an integer(32bit).

##### Encryption Matrix must be Inversible 

The matrix must be inversible. Meaning, it's determinant must have a gcd of 1 with the modulo base.

##### Random Bits Addition

Adding random bits inside the plaintext before encryption to remove lineary connection.

##### Digit to ASCII Mapping

For each digit we choose 6 different letters in the alphabet so it would be tougher to spot patterns in the ciphertext.

###### The Mapping:

- 0 : [b, M, Z, t, F, d]
- 1 : [w, A, i, Y, f, n]
- 2 : [h, p, G, k, s, D]
- 3 : [l, c, V, x, j, r]
- 4 : [q, L, U, e, y, P]
- 5 : [o, K, z, T, S, g]
- 6 : [J, W, v, a, R, H]
- 7 : [X, N, m, C, B, I]
- 8 : [Y, b, P, e, O, w]
- 9 : [F, d, V, u, Z, L]

### Thanks and Credit

Written by Omer Gindi.

A lot of thanks for Tony Patti for exposing me to the article and answering my many questions
about the world of applied cryptography.
