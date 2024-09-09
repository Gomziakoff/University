#include <fstream>
#include <iostream>
#include <openssl/evp.h>
#include <openssl/pem.h>
#include <openssl/rand.h>
#include <openssl/err.h>
#include <fcntl.h>
#include <stdlib.h>

void Encrypt() {
    EVP_PKEY* pubKey = NULL;
    EVP_PKEY_CTX* ctx;
    FILE* pubKey_file = NULL;
    std::vector<unsigned char> ctext, ptext;
    size_t outlen, inlen;

    // Загрузка открытого ключа
    pubKey_file = fopen("public.key", "rb");
    pubKey = PEM_read_PUBKEY(pubKey_file, NULL, NULL, NULL);
    fclose(pubKey_file);

    ctx = EVP_PKEY_CTX_new(pubKey, NULL);
    if (!ctx || EVP_PKEY_encrypt_init(ctx) <= 0) {
        ERR_print_errors_fp(stderr);
        exit(1);
    }

    int key_size = EVP_PKEY_get_size(pubKey);
    ctext.resize(key_size);
    ptext.resize(key_size);

    std::ofstream out("rsa.file", std::ios::binary);
    std::ifstream in("in.txt", std::ios::binary);

    while (in.read(reinterpret_cast<char*>(&ptext[0]), key_size - 11)) {
        inlen = in.gcount();
        if (EVP_PKEY_encrypt(ctx, ctext.data(), &outlen, ptext.data(), inlen) <= 0) {
            ERR_print_errors_fp(stderr);
            exit(1);
        }
        out.write(reinterpret_cast<char*>(&ctext[0]), outlen);
    }

    std::cout << "Содержимое файла in.txt было зашифровано и помещено в файл rsa.file" << std::endl;

    EVP_PKEY_CTX_free(ctx);
    EVP_PKEY_free(pubKey);
}

void Decrypt(char secret[]) {
    EVP_PKEY* privKey = NULL;
    EVP_PKEY_CTX* ctx;
    FILE* privKey_file;
    std::vector<unsigned char> ptext, ctext;
    size_t outlen, inlen;

    privKey_file = fopen("private.key", "rb");
    privKey = PEM_read_PrivateKey(privKey_file, NULL, NULL, secret);
    fclose(privKey_file);

    ctx = EVP_PKEY_CTX_new(privKey, NULL);
    if (!ctx || EVP_PKEY_decrypt_init(ctx) <= 0) {
        ERR_print_errors_fp(stderr);
        exit(1);
    }

    int key_size = EVP_PKEY_get_size(privKey);
    ptext.resize(key_size);
    ctext.resize(key_size);

    std::ofstream out("out.txt", std::ios::binary);
    std::ifstream in("rsa.file", std::ios::binary);

    while (in.read(reinterpret_cast<char*>(&ctext[0]), key_size)) {
        inlen = in.gcount();
        if (EVP_PKEY_decrypt(ctx, ptext.data(), &outlen, ctext.data(), inlen) <= 0) {
            ERR_print_errors_fp(stderr);
            exit(1);
        }
        out.write(reinterpret_cast<char*>(&ptext[0]), outlen);
    }

    std::cout << "Содержимое файла rsa.file было дешифровано и помещено в файл out.txt" << std::endl;

    EVP_PKEY_CTX_free(ctx);
    EVP_PKEY_free(privKey);
}
