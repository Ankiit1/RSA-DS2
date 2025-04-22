#include <iostream>
#include <cmath>
#include <cstdlib>
#include <ctime>

using namespace std;

const int FORBIDDEN_PRIMES[] = {2, 3, 5, 7, 11};

int gcd(int a, int b) {
    while (b != 0) {
        int temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

bool is_forbidden(int num) {
    for (int prime : FORBIDDEN_PRIMES) {
        if (num == prime) return true;
    }
    return false;
}

bool is_prime(int num) {
    if (num < 2) return false;
    for (int i = 2; i <= sqrt(num); i++) {
        if (num % i == 0) return false;
    }
    return true;
}

long long mod_pow(long long base, long long exp, long long mod) {
    long long result = 1;
    while (exp > 0) {
        if (exp % 2 == 1)
            result = (result * base) % mod;
        base = (base * base) % mod;
        exp /= 2;
    }
    return result;
}

int mod_inverse(int e, int phi) {
    for (int d = 12; d < phi; d++) {
        if ((e * d) % phi == 1) return d;
    }
    return -1;
}

void generate_rsa_keys(int &p, int &q, int &n, int &e, int &d) {
    if (!is_prime(p) || !is_prime(q) || p == q) {
        cout << "Invalid input! p and q must be distinct prime numbers." << endl;
        exit(EXIT_FAILURE);
    }

    n = p * q;
    int phi = (p - 1) * (q - 1);

    srand(time(0));
    do {
        e = 12 + rand() % (phi - 12);
    } while (gcd(e, phi) != 1 || is_forbidden(e));

    d = mod_inverse(e, phi);
    if (d == -1) {
        cout << "Error: Could not find modular inverse for 'e'. Exiting." << endl;
        exit(EXIT_FAILURE);
    }
}

int main() {
    int p, q, n, e, d;

    cout << "Enter a prime number (p): ";
    cin >> p;
    cout << "Enter another prime number (q): ";
    cin >> q;

    generate_rsa_keys(p, q, n, e, d);
    cout << "\nPublic Key (n, e): (" << n << ", " << e << ")";
    cout << "\nPrivate Key (n, d): (" << n << ", " << d << ")\n";

    int message;
    cout << "\nEnter a number to send (must be less than " << n << "): ";
    cin >> message;

    if (message >= n) {
        cout << "Message must be less than " << n << ". Exiting.\n";
        return 1;
    }

    int encrypted = mod_pow(message, e, n);
    cout << "\nEncrypted Message: " << encrypted;

    int signature = mod_pow(message, d, n);
    cout << "\nDigital Signature: " << signature;

    cout << "\n\nReceiver Side:";

    int decrypted = mod_pow(encrypted, d, n);
    cout << "\nDecrypted Message: " << decrypted;

    int verified_signature = mod_pow(signature, e, n);
    cout << "\nVerified Signature: " << verified_signature;

    if (verified_signature == decrypted) {
        cout << "\n\nSignature is VALID. Message is authentic and untampered.\n";
    } else {
        cout << "\n\nSignature is INVALID. Message may have been tampered with.\n";
    }

    return 0;
}
