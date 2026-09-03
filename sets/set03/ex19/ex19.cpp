#include "ex19.h"
#include "common.h"
#include "aes.h"
#include <algorithm>
#include <cctype>

namespace set03::ex19
{

    namespace
    {
        const std::vector<std::string> PLAINTEXTS_B64 = {
            "SSBoYXZlIG1ldCB0aGVtIGF0IGNsb3NlIG9mIGRheQ==",
            "Q29taW5nIHdpdGggdml2aWQgZmFjZXM=",
            "RnJvbSBjb3VudGVyIG9yIGRlc2sgYW1vbmcgZ3JleQ==",
            "RWlnaHRlZW50aC1jZW50dXJ5IGhvdXNlcy4=",
            "SSBoYXZlIHBhc3NlZCB3aXRoIGEgbm9kIG9mIHRoZSBoZWFk",
            "T3IgcG9saXRlIG1lYW5pbmdsZXNzIHdvcmRzLA==",
            "T3IgaGF2ZSBsaW5nZXJlZCBhd2hpbGUgYW5kIHNhaWQ=",
            "UG9saXRlIG1lYW5pbmdsZXNzIHdvcmRzLA==",
            "QW5kIHRob3VnaHQgYmVmb3JlIEkgaGFkIGRvbmU=",
            "T2YgYSBtb2NraW5nIHRhbGUgb3IgYSBnaWJl",
            "VG8gcGxlYXNlIGEgY29tcGFuaW9u",
            "QXJvdW5kIHRoZSBmaXJlIGF0IHRoZSBjbHViLA==",
            "QmVpbmcgY2VydGFpbiB0aGF0IHRoZXkgYW5kIEk=",
            "QnV0IGxpdmVkIHdoZXJlIG1vdGxleSBpcyB3b3JuOg==",
            "QWxsIGNoYW5nZWQsIGNoYW5nZWQgdXR0ZXJseTo=",
            "QSB0ZXJyaWJsZSBiZWF1dHkgaXMgYm9ybi4=",
            "VGhhdCB3b21hbidzIGRheXMgd2VyZSBzcGVudA==",
            "SW4gaWdub3JhbnQgZ29vZCB3aWxsLA==",
            "SGVyIG5pZ2h0cyBpbiBhcmd1bWVudA==",
            "VW50aWwgaGVyIHZvaWNlIGdyZXcgc2hyaWxsLg==",
            "V2hhdCB2b2ljZSBtb3JlIHN3ZWV0IHRoYW4gaGVycw==",
            "V2hlbiB5b3VuZyBhbmQgYmVhdXRpZnVsLA==",
            "U2hlIHJvZGUgdG8gaGFycmllcnM/",
            "VGhpcyBtYW4gaGFkIGtlcHQgYSBzY2hvb2w=",
            "QW5kIHJvZGUgb3VyIHdpbmdlZCBob3JzZS4=",
            "VGhpcyBvdGhlciBoaXMgaGVscGVyIGFuZCBmcmllbmQ=",
            "V2FzIGNvbWluZyBpbnRvIGhpcyBmb3JjZTs=",
            "SGUgbWlnaHQgaGF2ZSB3b24gZmFtZSBpbiB0aGUgZW5kLA==",
            "U28gc2Vuc2l0aXZlIGhpcyBuYXR1cmUgc2VlbWVkLA==",
            "U28gZGFyaW5nIGFuZCBzd2VldCBoaXMgdGhvdWdodC4=",
            "VGhpcyBvdGhlciBtYW4gSSBoYWQgZHJlYW1lZA==",
            "QSBkcnVua2VuLCB2YWluLWdsb3Jpb3VzIGxvdXQu",
            "SGUgaGFkIGRvbmUgbW9zdCBiaXR0ZXIgd3Jvbmc=",
            "VG8gc29tZSB3aG8gYXJlIG5lYXIgbXkgaGVhcnQs",
            "WWV0IEkgbnVtYmVyIGhpbSBpbiB0aGUgc29uZzs=",
            "SGUsIHRvbywgaGFzIHJlc2lnbmVkIGhpcyBwYXJ0",
            "SW4gdGhlIGNhc3VhbCBjb21lZHk7",
            "SGUsIHRvbywgaGFzIGJlZW4gY2hhbmdlZCBpbiBoaXMgdHVybiw=",
            "VHJhbnNmb3JtZWQgdXR0ZXJseTo=",
            "QSB0ZXJyaWJsZSBiZWF1dHkgaXMgYm9ybi4="};

        std::vector<unsigned char> KEY;
        bool initialized = false;

        void ensure_init()
        {
            if (!initialized)
            {
                KEY = rand_bytes(16);
                initialized = true;
            }
        }

        double score_column(const std::string &text, size_t position)
        {
            double score = score_english(text);
            const bool uppercase_expected = position == 0;

            // score_english intentionally ignores case. Use the position in a
            // plaintext line to resolve the otherwise indistinguishable case
            // choice without changing the shared scorer.
            for (unsigned char c : text)
            {
                if (!std::isalpha(c))
                    continue;

                const bool uppercase = std::isupper(c);
                score += uppercase == uppercase_expected ? 0.01 : -0.01;
            }

            return score;
        }
    }

    std::vector<std::vector<unsigned char>> encrypt_plaintexts()
    {
        ensure_init();
        std::vector<std::vector<unsigned char>> ciphertexts;

        for (const auto &b64 : PLAINTEXTS_B64)
        {
            auto plaintext = base64_to_bytes(b64);
            std::vector<uint8_t> pt(plaintext.begin(), plaintext.end());
            auto ct = aes::ctr(pt, KEY, 0); // Fixed nonce = 0
            ciphertexts.push_back(std::vector<unsigned char>(ct.begin(), ct.end()));
        }

        return ciphertexts;
    }

    std::vector<std::string> break_fixed_nonce_ctr(const std::vector<std::vector<unsigned char>> &ciphertexts)
    {
        if (ciphertexts.empty())
            return {};

        // Find max length
        size_t max_len = 0;
        for (const auto &ct : ciphertexts)
            max_len = std::max(max_len, ct.size());

        // Recover keystream byte by byte using frequency analysis
        std::vector<unsigned char> keystream(max_len, 0);

        for (size_t pos = 0; pos < max_len; ++pos)
        {
            // Gather all bytes at this position
            std::vector<unsigned char> column;
            for (const auto &ct : ciphertexts)
            {
                if (pos < ct.size())
                    column.push_back(ct[pos]);
            }

            // Find best keystream byte using English frequency scoring
            double best_score = -1e9;
            unsigned char best_key = 0;

            for (int key = 0; key < 256; ++key)
            {
                std::string decrypted;
                for (unsigned char c : column)
                    decrypted += (char)(c ^ key);

                double score = score_column(decrypted, pos);
                if (score > best_score)
                {
                    best_score = score;
                    best_key = (unsigned char)key;
                }
            }

            keystream[pos] = best_key;
        }

        // Decrypt all ciphertexts
        std::vector<std::string> plaintexts;
        for (const auto &ct : ciphertexts)
        {
            std::string pt;
            for (size_t i = 0; i < ct.size(); ++i)
                pt += (char)(ct[i] ^ keystream[i]);
            plaintexts.push_back(pt);
        }

        return plaintexts;
    }

} // namespace set03::ex19
