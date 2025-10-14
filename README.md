# Galois Field Hill Cipher


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
| `-f`, `--prime-field`           | Specify the prime field (optional, default: `16777619`).                                                         |
| `-r`, `--random-bits`           | Specify the number of random bits to add between bytes (optional, default: `2`).                                 |
| `-a`, `--ascii-mapping-letters` | Specify the number of letters mapped for each digit in the ASCII mapping (optional, default: `5`).                                                      |
| `-e`, `--error-vectors` | Specify the number of letters mapped for each digit in the ASCII mapping (optional, default: `5`).                                                      |
| `-l`, `--log`                   | Specify the log file.                                                                                 |
| `-m`, `--mode`                  | Specify the mode of operation (`kg`, `dkg`, `e`, `d`, `kge`, `kgd`).                                                |
| `-v`, `--verbose`               | Enable verbose output (optional).                                                                                |

#### Notes

* **Text vs Binary encryption mode** – Defines how the cipher operates:
  * **Text encryption**: less compact but more secure, with reduced entropy.
  * **Binary encryption**: much more compact, optimized for storage and performance.

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

When storing encrypted data in binary format, the program calculates the minimum number of bits required for each element within the chosen finite field and stores the elements consecutively, without padding, to ensure compact storage.

###### ASCII Mapping

For text storage, there is mapping between the digits of the ciphertext, each number of the GF fits inside 8 digits.
The mapping has a chosen variant(Same digit maps to few different letters) of 1 to 6 which helps reduce the entropy of the ciphertext.

### Thanks and Credit

Written by Omer Gindi.

A lot of thanks for Tony Patti for exposing me to the article and answering my many questions
about the world of applied cryptography.
