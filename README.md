# Post Quantum Hill Cryptosystem with Affine Transformation and Large Galois Fields


**This implementation is for EDUCATIONAL PURPOSES ONLY and should NOT be used in any real-world environment.**


## Usage

### Modes

| **Mode** | **Name**                      | **Description**                                                      | **Required Arguments**                                                                                                      | **Optional Arguments**                                                  | **Example Command**                                                                                                  |
| -------- | ----------------------------- | -------------------------------------------------------------------- | --------------------------------------------------------------------------------------------------------------------------- | ----------------------------------------------------------------------- | -------------------------------------------------------------------------------------------------------------------- |
| `kg`     | **Key Generation**            | Generates an encryption key matrix.                                  | `-m kg` `-o <output_file>` `-d <dimension>`                                                                                 | `-f <prime_field>` `-r <random_bits>` `-v`                              | `GaloisFieldHillCipher -m kg -o key.bin -d 4 -v`                                                                     |
| `dkg`    | **Decryption Key Generation** | Generates the decryption key matrix from an existing encryption key. | `-m dkg` `-k <key_file>` `-o <output_file>`                                                                                 | `-v`                                                                    | `GaloisFieldHillCipher -m dkg -k key.bin -o decryption_key.bin -v`                                                   |
| `e`      | **Encrypt (Text/Binary)**     | Encrypts an input file using a key file.                             | `-m e` `-i <input_file>` `-o <output_file>` `-k <key_file>`                                                                 | `-v` `-r <random_bits>` `-a <ascii_mapping_letters>`                    | `GaloisFieldHillCipher -m e -i plaintext.txt -o encrypted.bin -k key.bin -v`                                         |
| `d`      | **Decrypt (Text/Binary)**     | Decrypts an encrypted file using a key file.                         | `-m d` `-i <input_file>` `-o <output_file>` `-k <key_file>`                                                                 | `-v`                                                                    | `GaloisFieldHillCipher -m d -i encrypted.bin -o decrypted.txt -k decryption_key.bin -v`                              |
| `kge`    | **Generate and Encrypt**      | Generates a key and encrypts a file in one step.                     | `-m kge` `-i <input_file>` `-o <output_file>` `-k <key_output_file>` `-d <dimension>`                                       | `-r <random_bits>` `-f <prime_field>` `-a <ascii_mapping_letters>` `-v` | `GaloisFieldHillCipher -m kge -i plaintext.txt -o encrypted.bin -k key.bin -d 4 -v`                                  |
| `kgd`    | **Generate and Decrypt**      | Generates a decryption key and decrypts a file in one step.          | `-m kgd` `-i <input_file>` `-o <output_file>` `-k <encryption_key_file>` `-y <decryption_key_output_file>` `-d <dimension>` | `-v`                                                                    | `GaloisFieldHillCipher -m kgd -i encrypted.bin -o decrypted.txt -k encryption_key.bin -y decryption_key.bin -d 4 -v` |

---


### Flags

| **Flag**                        | **Description**                                                                                                  |
| ------------------------------- | ---------------------------------------------------------------------------------------------------------------- |
| `-i`, `--input`                 | Specify the input file(.bin or .txt).    |
| `-o`, `--output`                | Specify the output file (.bin or .txt).                                                                |
| `-k`, `--key`                   | Specify the key file. |
| `-y`, `--decryption-key-output` | Specify the decryption key output file.                                 |
| `-d`, `--dimension`             | Specify the key matrix dimension.                       |
| `-f`, `--prime-pield`           | Specify the prime field (optional, default: `16777619`).                                                         |
| `-r`, `--random-bits`           | Specify the number of random bits to add between bytes (optional, default: `2`).                                 |
| `-a`, `--ascii-mapping-letters` | Specify the number of letters mapped for each digit in the ASCII mapping (optional, default: `5`).                                                      |
| `-e`, `--error-vectors` | Specify the number of error vectors to add to the matrix-vector multiplication (optional, default: `5`).                                                      |
| `-l`, `--log`                   | Specify the log file.                                                                                 |
| `-m`, `--mode`                  | Specify the mode of operation (`kg`, `dkg`, `e`, `d`, `kge`, `kgd`).                                                |
| `-v`, `--verbose`               | Enable verbose output (optional).                                                                                |

#### Notes

* **Text vs Binary encryption mode** – Defines how the cipher operates:
  * **Text encryption**: less compact but more secure, with reduced entropy.
  * **Binary encryption**: much more compact, optimized for storage and performance.

## Overview 

An implimentation of a modern extension of the Hill cipher as described by Rodney Cooper in 1980.

Based on the second version of the paper "An interesting example at the intersection
of Matrix Mathematics and Cryptography" by Tony Patti.

I strongly recommand reading the article, here is a link for the article in glassblower: [An interesting example at the intersection of Matrix Mathematics and Cryptography, by Tony Patti](https://www.glassblower.info/crypto/an-interesting-example-at-the-intersection-of-matrix-mathematics-and-cryptography-version-2.pdf)

The modern extension makes the cryptosystem probably secure for modern cryptographic standards(NOT TESTED ENOUGH - DO NOT USE IT FOR ANY REAL-WORLD ENVIRONMENT SYSTEMS). And utilizes a post quantum encryption layer that is makes this cipher theoretically secure against quantom computer's attacks.

### The Original Hill Cipher

The Hill cipher is based on linear algebra.
It utilizes invertible matrixes to encrypt blocks of known length - it is a polygraphic substitution cipher.

##### The Encryption Process

Each vector of blocks(of size n) is multiplied with an invertible matrix(of size n x n). 
Afterwards, we calculate the result mod the finite field.

##### The Decyription Process

Each ciphertext vector is multiplied with the inverse of the encryption matrix. 
Afterwards, we calculate the result mod the finite field.

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
A field of order n would contain the numbers {0, 1, 2, ..., n - 1}

##### Matrix Multipication With Vector

Each matrix row is multiplied element-wise with the plaintext vector. The results are summed to produce the final vector.

Example:

Encryption Matrix: 

| C1 | C2 | C3 | 
|----|----|----|
| 2  | 3  | 4  | 
| 6  | 8  | 9  | 
| 4  | 5  | 2  |

Plaintext Vector: 

|     | 
|-----| 
| 3   | 
| 5   | 
| 7   |

Multiplication and Sum:

Row 1: 2×3 + 3×5 + 4×7 = 49

Row 2: 6×3 + 8×5 + 9×7 = 121

Row 3: 4×3 + 5×5 + 2×7 = 51


Resulting Vector: 

|     | 
|-----| 
| 49  | 
| 121 | 
| 51  |


##### LWE (Learning With Errors)

The Learning With Errors (LWE) problem is an interesting idea for post-quantum cryptography. It's based on the idea that while solving a system of linear equations is computationally easy, it becomes very difficult when a small amount of random "noise" or "error" is introduced to each equation.

The core relationship in LWE is defined by the equation:
$$b = (A \cdot s + e) \pmod{q}$$

The error vector "e" hides the linear equations and therefore makes it very hard to crack.

***

### Implementation

##### Chosen Galois Field

The default field is GF(16,777,619),
You can change it at any time using the main argument -f/--prime-field.

###### Constraints

The field must be prime and it is advisble to use a large field.

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

When storing encrypted data in binary format, the program calculates the minimum number of bits required for each element within the chosen finite field and stores the elements consecutively, without padding, to ensure compact storage.

###### ASCII Mapping

For text storage, there is mapping between the digits of the ciphertext, each number of the GF fits inside 8 digits.
The mapping has a chosen variant(Same digit maps to few different letters) ,that can be modified in the main argument -a/--ascii-mapping-letters, of 1 to 6 which helps reduce the entropy of the ciphertext.

###### ASCII Permutation

After the mapping, each group of letters that represent 
a single field element is getting shuffled for adding another layer
of security.

### Codebase

#### Testing

There are tests covering:
- Ciphertext Expansion
- Block Dividing
- Padding
- ASCII Mapping
- Field Operations
- Matrix Determinant
- Minor Matrix Crafting
- GCD
- Is Matrix Invertible
- Matrix Inverse Calaculation
- Matrix and Vector Multiplication - uint8_t vector
- Matrix and Vector Multiplication - int64_t vector

#### CI

There is a CI pipeline that builds and tests linux and windows versions in Release and Debug configurations.

In the linux CI there is a call to valgrind in each mode to test the program for any memory leak.

There is also a call to CodeQL static analyzer to catch security vunrabilities on a weekly basis. 

##### Hardening

The CI is hardened by:
- Actions are pinned to a specific SHA Commit.
- Verifing each artifact with it's SHA256 checksum before trusting.
- Using specific job image.

#### Logging

There is a logger that writes to the console if the verbose flag is on(Can be modified using the main argument -v/--verbose) and to a specified log file that can be modified using the main argument -l/--log. 

### Thanks and Credit

Written by Omer Gindi.

A lot of thanks for Tony Patti for exposing me to the article and answering my many questions
about the world of applied cryptography.
