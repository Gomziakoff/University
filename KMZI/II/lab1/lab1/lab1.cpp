#include <openssl/evp.h>
#include <openssl/rand.h>
#include <openssl/provider.h>
#include <openssl/err.h>
#include <iostream>
#include <vector>
#include <cstring>
#include <stdexcept>

const int KEY_SIZE = 32; // Размер ключа для AES-256
const int BLOCK_SIZE = 16; // Размер блока для AES

std::pair<std::vector<unsigned char>, std::vector<unsigned char>> split_vector(const std::vector<unsigned char>& original) {
    
    size_t mid = original.size() / 2;

    std::vector<unsigned char> first_half(original.begin(), original.begin() + mid);
    std::vector<unsigned char> second_half(original.begin() + mid, original.end());

    return { first_half, second_half };
}

std::vector<unsigned char> getSubVector(const std::vector<unsigned char>& vec, size_t start, size_t end) {
    // Проверяем корректность индексов
    if (start > end || end > vec.size()) {
        throw std::out_of_range("Некорректные индексы");
    }

    // Используем итераторы для создания подвектора
    return std::vector<unsigned char>(vec.begin() + start, vec.begin() + end);
}

std::vector<unsigned char> xorVectors(const std::vector<unsigned char>& vec1, const std::vector<unsigned char>& vec2) {
    // Проверяем, что размеры векторов одинаковы
    if (vec1.size() != vec2.size()) {
        throw std::invalid_argument("Векторы должны быть одинакового размера");
    }

    // Результирующий вектор для хранения результата XOR
    std::vector<unsigned char> result(vec1.size());

    // Выполняем побитовый XOR для каждого элемента
    for (size_t i = 0; i < vec1.size(); ++i) {
        result[i] = vec1[i] ^ vec2[i];
    }

    return result;
}

std::vector<unsigned char> generate_random_number(int size) {
    std::vector<unsigned char> num(size);
    if (!RAND_bytes(num.data(), size)) {
        throw std::runtime_error("Не удалось сгенерировать случайное число");
    }
    return num;
}

std::vector<unsigned char> generate_random_key(size_t key_size) {
    std::vector<unsigned char> key(key_size);

    if (RAND_bytes(key.data(), key_size) != 1) {
        // Обработка ошибки
        throw std::runtime_error("Error generating random key");
    }

    return key;
}

std::vector<unsigned char> aes_encrypt(const std::vector<unsigned char>& key, const std::vector<unsigned char>& plaintext) {
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    std::vector<unsigned char> ciphertext(plaintext.size() + BLOCK_SIZE);
    std::vector<unsigned char> iv(BLOCK_SIZE);

    RAND_bytes(iv.data(), BLOCK_SIZE);

    int len;
    int ciphertext_len;

    if (!EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key.data(), iv.data())) {
        throw std::runtime_error("Ошибка инициализации шифрования");
    }

    if (!EVP_EncryptUpdate(ctx, ciphertext.data(), &len, plaintext.data(), plaintext.size())) {
        throw std::runtime_error("Ошибка шифрования");
    }
    ciphertext_len = len;

    if (!EVP_EncryptFinal_ex(ctx, ciphertext.data() + len, &len)) {
        throw std::runtime_error("Ошибка завершения шифрования");
    }
    ciphertext_len += len;

    EVP_CIPHER_CTX_free(ctx);

    ciphertext.resize(ciphertext_len);
    ciphertext.insert(ciphertext.end(), iv.begin(), iv.end());
    return ciphertext;
}

std::vector<unsigned char> aes_decrypt(const std::vector<unsigned char>& key, const std::vector<unsigned char>& ciphertext) {
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    std::vector<unsigned char> plaintext(ciphertext.size());
    std::vector<unsigned char> iv(BLOCK_SIZE);
    std::memcpy(iv.data(), ciphertext.data() + ciphertext.size() - BLOCK_SIZE, BLOCK_SIZE);

    int len;
    int plaintext_len;

    if (!EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key.data(), iv.data())) {
        throw std::runtime_error("Ошибка инициализации дешифрования");
    }

    if (!EVP_DecryptUpdate(ctx, plaintext.data(), &len, ciphertext.data(), ciphertext.size() - BLOCK_SIZE)) {
        throw std::runtime_error("Ошибка дешифрования");
    }
    plaintext_len = len;

    if (!EVP_DecryptFinal_ex(ctx, plaintext.data() + len, &len)) {
        throw std::runtime_error("Ошибка завершения дешифрования");
    }
    plaintext_len += len;

    EVP_CIPHER_CTX_free(ctx);

    plaintext.resize(plaintext_len);
    return plaintext;
}

std::vector<unsigned char> EKE_demo(std::vector<unsigned char> password,std::vector<unsigned char> key_) {
    // Алиса шифрует ключ и отправляет
    auto ekey = aes_encrypt(password, key_);
    std::cout << "encrypted key_:\n";
    for (auto& byte : ekey) std::cout << std::hex << (int)byte;
    std::cout << std::endl << std::endl;

    // Боб расшифровывает сообщение
    std::vector<unsigned char> bobsKey_ = aes_decrypt(password, ekey);
    std::cout << "decrypted key_:\n";
    for (auto& byte : bobsKey_) std::cout << std::hex << (int)byte;
    std::cout << std::endl << std::endl;

    // Боб генерирует сеансовый ключ
    std::vector<unsigned char> bobsKey = generate_random_key(32);
    std::cout << "key:\n";
    for (auto& byte : bobsKey) std::cout << std::hex << (int)byte;
    std::cout << std::endl << std::endl;

    // Боб шифрует открытым ключем сеансовый, а затем шифрует паролем и отправляет
    std::vector<unsigned char> ebobsKey = aes_encrypt(password, aes_encrypt(bobsKey_, bobsKey));
    std::cout << "encrypted key:\n";
    for (auto& byte : ebobsKey) std::cout << std::hex << (int)byte;
    std::cout << std::endl << std::endl;
    /*--------------------------------------------------------------------------*/
    // Алиса расшифровывает сообщение и получает сеансовый ключ
    std::vector<unsigned char> alicesKey = aes_decrypt(key_, aes_decrypt(password, ebobsKey));
    std::cout << "decrypted key:\n";
    for (auto& byte : alicesKey) std::cout << std::hex << (int)byte;
    std::cout << std::endl << std::endl;

    // Алиса генерирует случайную строку и случайное чило
    std::vector<unsigned char> Ra = generate_random_key(32);
    std::vector<unsigned char> Sa = generate_random_number(32);
    auto RaSa = Ra;
    RaSa.insert(RaSa.end(), Sa.begin(), Sa.end());
    std::cout << "Ra + Sa:\n";
    for (auto& byte : RaSa) std::cout << std::hex << (int)byte;
    std::cout << std::endl << std::endl;

    // Алиса шифрует строку и отправляет
    std::vector<unsigned char> eRa = aes_encrypt(alicesKey, RaSa);
    std::cout << "encrypted Ra + Sa:\n";
    for (auto& byte : eRa) std::cout << std::hex << (int)byte;
    std::cout << std::endl << std::endl;

    // Боб расшифровывает сообщение и получает строку и число алисы
    std::vector<unsigned char> BobsRa = split_vector(aes_decrypt(bobsKey, eRa)).first;
    std::vector<unsigned char> BobsSa = split_vector(aes_decrypt(bobsKey, eRa)).second;
    std::cout << "decrypted Ra + Sa:\n";
    for (auto& byte : BobsRa) std::cout << std::hex << (int)byte;
    std::cout << std::endl;
    for (auto& byte : BobsSa) std::cout << std::hex << (int)byte;
    std::cout << std::endl << std::endl;

    // Боб генерирует свою случайную строку и число, шифрует и отправляет
    std::vector<unsigned char> Rb = generate_random_key(32);
    std::vector<unsigned char> Sb = generate_random_key(32);

    std::cout << "Rb:\n";
    for (auto& byte : Rb) std::cout << std::hex << (int)byte;
    std::cout << std::endl << std::endl;

    auto comb = BobsRa;
    comb.insert(comb.end(), Rb.begin(), Rb.end());
    comb.insert(comb.end(), Sb.begin(), Sb.end());
    auto eBobsRaRb = aes_encrypt(bobsKey, comb);

    std::cout << "encrypted Ra + Rb:\n";
    for (auto& byte : eBobsRaRb) std::cout << std::hex << (int)byte;
    std::cout << std::endl << std::endl;

    // Алиса получает строки и расшифровывает их
    auto aliceRaRb = aes_decrypt(alicesKey, eBobsRaRb);
    std::cout << "decrypted Ra + Rb:\n";
    for (auto& byte : aliceRaRb) std::cout << std::hex << (int)byte;
    std::cout << std::endl << std::endl;
    std::vector<unsigned char> deRa = getSubVector(aliceRaRb,0,32);
    std::vector<unsigned char> deRb = getSubVector(aliceRaRb, 32, 64);
    std::vector<unsigned char> deSb = getSubVector(aliceRaRb, 64, 96);

    if (!(deRa == Ra)) return{};

    // Алиса шифрует Rb и отправляет
    std::vector<unsigned char> eRb = aes_encrypt(alicesKey, deRb);

    // Боб расшифровывает и сравнивает значения и, если все ок то ок
    if (aes_decrypt(bobsKey, eRb) == Rb) {
        std::cout << "You get private key!!!\n\n\n";
        return xorVectors(Sa,Sb);
    }
    else return{};
}

void handleErrors() {
    ERR_print_errors_fp(stderr);
    abort();
}

std::vector<unsigned char> encrypt(const std::vector<unsigned char>& plaintext, const std::vector<unsigned char>& key) {
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    if (!ctx) handleErrors();

    std::vector<unsigned char> ciphertext(plaintext.size() + EVP_CIPHER_block_size(EVP_rc2_ecb()));

    int len;
    int ciphertext_len;

    if (1 != EVP_EncryptInit_ex(ctx, EVP_rc2_ecb(), nullptr, key.data(), nullptr)) {
        handleErrors();
    }

    if (1 != EVP_EncryptUpdate(ctx, ciphertext.data(), &len, plaintext.data(), plaintext.size())) {
        handleErrors();
    }
    ciphertext_len = len;

    if (1 != EVP_EncryptFinal_ex(ctx, ciphertext.data() + len, &len)) {
        handleErrors();
    }
    ciphertext_len += len;

    ciphertext.resize(ciphertext_len);
    EVP_CIPHER_CTX_free(ctx);

    return ciphertext;
}

std::vector<unsigned char> decrypt(const std::vector<unsigned char>& ciphertext, const std::vector<unsigned char>& key) {
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    if (!ctx) handleErrors();

    std::vector<unsigned char> plaintext(ciphertext.size());

    int len;
    int plaintext_len;

    if (1 != EVP_DecryptInit_ex(ctx, EVP_rc2_ecb(), nullptr, key.data(), nullptr)) {
        handleErrors();
    }

    if (1 != EVP_DecryptUpdate(ctx, plaintext.data(), &len, ciphertext.data(), ciphertext.size())) {
        handleErrors();
    }
    plaintext_len = len;

    if (1 != EVP_DecryptFinal_ex(ctx, plaintext.data() + len, &len)) {
        handleErrors();
    }
    plaintext_len += len;

    plaintext.resize(plaintext_len);
    EVP_CIPHER_CTX_free(ctx);

    return plaintext;
}


int main() {
    OSSL_PROVIDER* default_provider = OSSL_PROVIDER_load(NULL, "default");
    OSSL_PROVIDER* legacy = OSSL_PROVIDER_load(NULL, "legacy");
    if (!default_provider || !legacy) {
        std::cerr << "Не удалось загрузить провайдеры" << std::endl;
        handleErrors();
    }

    std::vector<unsigned char> password = { 'p', 'a', 's', 's', 'w', 'o', 'r', 'd', '1', '2', '3', '!'};
    bool work = 0;
    std::vector<unsigned char> private_key;
    while (!work) {
        try {
            std::vector<unsigned char> open_key = generate_random_key(32);
            private_key = EKE_demo(password, open_key);  // Используйте глобальную переменную private_key
            work = true;  // Если всё успешно, выходим из цикла

        }
        catch (const std::exception& e) {
            std::cerr << "Произошла ошибка: " << e.what() << std::endl;
            system("cls");  // Очистка консоли в Windows (для Linux используйте "clear")
        }
    }

    for (auto& byte : private_key) std::cout << std::hex << (int)byte;
    std::cout << std::endl;

    std::vector<unsigned char> text = { 'h','e','l','l','o',' ','w','o','r','l','d' };

    // Шифрование
    std::vector<unsigned char> ciphertext = encrypt(text , private_key);
    for (auto& byte : ciphertext) std::cout << std::hex << (int)byte;
    std::cout << std::endl;
    // Расшифрование
    std::vector<unsigned char> decryptedtext = decrypt(ciphertext, private_key);
    for (auto& byte : decryptedtext) std::cout << byte;
}
