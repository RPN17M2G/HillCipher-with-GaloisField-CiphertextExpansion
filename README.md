# Galois Field Hill Cipher


**This implementation is for EDUCATIONAL PURPOSES ONLY and should NOT be used in any real-world environment.**


## Usage

### Modes

| Mode | Description | Required Flags |
|------|-------------|----------------|
| `ek` | Key generation | `-o <output_file> -d <dimension>` `[-v]` |
| `dk` | Decryption key generation | `-k <key> -o <output_file> -d <dimension>` `[-v]` |
| `e`  | Encrypt | `-i <input_file> -o <output_file> -k <key> -d <dimension>` `[-v]` |
| `d`  | Decrypt | `-i <input_file> -o <output_file> -k <key> -d <dimension>` `[-v]` |
| `ge` | Generate key and encrypt | `-i <input_file> -o <output_file> -k <key> -d <dimension>` `[-v]` |
| `gd` | Generate key and decrypt | `-i <input_file> -o <output_file> -k <key> -d <dimension>` `[-v]` |

### Flags

| Flag | Description |
|------|-------------|
| `-i`, `--input_file`    | Path to input file |
| `-o`, `--output_file`   | Path to output file |
| `-d`, `--dimension`     | Matrix/key dimension (`uint32_t`) |
| `-v`, `--verbose`       | Enable verbose output |
| `-m`, `--mode`          | Cipher mode of operation |
| `-k`, `--key`           | Path to key file |


## Overview 

An implimentation of the Hill cipher extended by the use of Galois fields as described by Rodney Cooper in 1980.

Based on the article "An interesting example at the intersection
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

**Example:**

*Encryption Matrix*:
| C1 | C2 | C3 |
|---|---|---|
| 2 | 3 | 4 |
| 6 | 8 | 9 |
| 4 | 5 | 2 |

*Plaintext Vector*:
| C1 |
|---|
| 3 |
| 5 |
| 7 |

Multiplication A * B:
| C1 | C2 | C3 |
|---|---|---|
| 2*3 | 3*5 | 4*7 |
| 6*3 | 8*5 | 9*7 |
| 4*3 | 5*5 | 2*7 |

Result:
| C1 |
|---|
| 49 |
| 121 |
| 51 |

### Implementation

##### Chosen Galois Field

The chosen field is GF(16,777,619)

###### Requirements

- Prime Number for defined modulo P, and exactly P number of elements.

##### Encryption Matrix must be Inversible 

The matrix must be inversible. Meaning, it's determinant must have a gcd of 1 with the modulo base.

##### Ciphertext Expansion

Adding random bits inside the plaintext before encryption to remove lineary connection.
It is called that way because the ciphertext is larger than the plaintext because of the insertion of the random bits.
In this specific implementation the bits are added after each byte, which creates the following sequence:

in case of 2 bits insertion:
! - Byte break


 1 0 1 0 1 0 1 0 !    | 0 1           | 0 0 0 0 0 0 ! 0 0     | 1 1           | 0 0 1 1 ...
 
 first plaintext byte | 2 random bits | second plaintext byte | 2 random bits | third plaintext byte...

##### Output and Input formats

Determinated by the extension of the output file. 

###### Binary Compact Storage

My chosen GF is 25bits long, what means that every member of this GF fits inside 24bits == 3bytes.
So there is an option to store the ciphertext in blocks of 3bytes each in the most compact way possible.

###### ASCII Mapping

For text storage, there is mapping between the digits of the ciphertext, each number of the GF fits inside 8 digits.
The mapping has a variant(Same digit maps to few different letters) of 5 which helps reduce the entropy of the ciphertext.

### Thanks and Credit

Written by Omer Gindi.

A lot of thanks for Tony Patti for exposing me to the article and answering my many questions
about the world of applied cryptography.
