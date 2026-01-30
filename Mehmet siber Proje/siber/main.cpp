#include <iostream>
#include <string>
#include <vector>
#include <cstdint>

using namespace std;
using ull = unsigned long long;

// Hızlı modüler üs alma (modpow)
ull modpow(ull base, ull exp, ull mod) {
    ull result = 1;
    base %= mod;
    while (exp > 0) {
        if (exp & 1) {
            result = (__int128)result * base % mod;
        }
        base = (__int128)base * base % mod;
        exp >>= 1;
    }
    return result;
}

// Modüler ters (extended Euclidean)
ull mod_inverse(ull a, ull m) {
    ull m0 = m, t, q;
    ull x0 = 0, x1 = 1;
    if (m == 1) return 0;
    while (a > 1) {
        q = a / m;
        t = m;
        m = a % m; a = t;
        t = x0;
        x0 = x1 - q * x0;
        x1 = t;
    }
    if (x1 < 0) x1 += m0;
    return x1;
}

int main() {
    ull p, q, e;

    cout << "RSA Anahtar Üretimi (Kullanıcı Girişli)\n";
    cout << "=====================================\n\n";

    cout << "Asal sayı p'yi girin: ";
    cin >> p;

    cout << "Asal sayı q'yu girin: ";
    cin >> q;

    cout << "Public exponent e'yi girin (genelde 3, 17, 65537 gibi): ";
    cin >> e;

    ull n = p * q;
    ull phi = (p - 1) * (q - 1);
    ull d = mod_inverse(e, phi);

    cout << "\nHesaplanan RSA Parametreleri:\n";
    cout << "p   = " << p << endl;
    cout << "q   = " << q << endl;
    cout << "n   = " << n << endl;
    cout << "φ(n)= " << phi << endl;
    cout << "e   = " << e << endl;
    cout << "d   = " << d << "  (e'nin mod φ(n) tersi)\n\n";

    // Kısa metin girişi
    string plaintext;
    cout << "Şifrelenecek kısa metni girin (ör: Merhaba RSA): ";
    cin.ignore();           // önceki enter'ı temizle
    getline(cin, plaintext);

    cout << "\nOrijinal Metin (Plaintext): " << plaintext << endl;

    vector<ull> ciphertext;
    cout << "Şifrelenmiş Bloklar (her karakter bir blok):\n";
    for (char ch : plaintext) {
        ull m = static_cast<unsigned char>(ch);
        if (m >= n) {
            cout << "Uyarı: Karakter '" << ch << "' (ASCII " << (int)m << ") n'den büyük! Şifreleme başarısız olabilir.\n";
        }
        ull c = modpow(m, e, n);
        ciphertext.push_back(c);
        cout << c << " ";
    }
    cout << "\n\n";

    // Deşifreleme
    string decrypted;
    cout << "Deşifrelenmiş Metin: ";
    for (ull c : ciphertext) {
        ull m = modpow(c, d, n);
        decrypted += static_cast<char>(m);
    }
    cout << decrypted << endl;

    // Doğrulama
    if (decrypted == plaintext) {
        cout << "\nBaşarılı: Deşifrelenmiş metin orijinal metin ile tamamen aynıdır.\n";
    } else {
        cout << "\nHata: Deşifreleme başarısız! (muhtemelen n çok küçük veya karakterler taşmış)\n";
    }

    cout << "\nNot: Daha büyük metinler / gerçek kullanım için 2048+ bit anahtar ve padding (OAEP) gerekir.\n";

    return 0;
}
