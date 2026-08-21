#include "BSD.h"
#include "CPU.h"
#include <iostream>
#include <fstream>
#include <string>
#include <cstdint>
using namespace std;

BSD bsd;

uint8_t DEBUG_BSD(string KOD) {
    ifstream File(KOD);

    if (!File.is_open()) {
        system("clear");
        cout << "HATA: Dosya Açılamadı!\n\nHata Kodu: 0x0006" << endl;
        return 0x06;
    }

    if (File.fail() && !File.is_open()) {
        system("clear");
        cout << "HATA: Dosya bulunamadı!\n\nHata Kodu: 0x0007" << endl;
        return 0x07;
    }

    if (File.fail()) {
        system("clear");
        cout << "HATA: Dosyada bir hata meydana geldi! Bu hata \"Okuma\" ve \"Yazma\" veya beklenen veri türünün gelmemesi olabilir.\n\nHata Kodu: 0x0008" << endl;
        File.close();
        return 0x08;
    }

    if (File.bad()) {
        system("clear");
        cout << "HATA: Dosyada kritik bir hata meydana geldi!\n\nHata Kodu: 0x0009" << endl;
        File.close();
        return 0x09;
    }

    if (File.good()) {
        cout << "\nDosya başarıyla açıldı & başlatıldı." << endl;
    }

    File.close();
    return 0;
}

uint8_t SGET_BIN(string KOD) {
    if (DEBUG_BSD(KOD) != 0) return DEBUG_BSD(KOD);
    ifstream File(KOD);

    string satir;
    size_t counter = 0;
    bsd.pc = 0;

    while (getline(File, satir)) {
        if (bsd.pc == 1048576) return 0xFF;
        while (satir.empty() == false) {
            size_t pos = satir.find('|');
            if (pos != string::npos) {
                counter++;
                if (counter == 4) break;
                if (counter == 1) bsd.OPCODE[bsd.pc] = static_cast<uint8_t>(stoi(satir.substr(0, pos), nullptr, 10));
                if (counter == 2) bsd.DATA1[bsd.pc] = static_cast<uint8_t>(stoi(satir.substr(0, pos), nullptr, 10));
                if (counter == 3) bsd.DATA2[bsd.pc] = stoi(satir.substr(0, pos), nullptr, 10);
                satir.erase(0, pos + 1);
            } else {
                break;
            }
        }
        counter = 0;
        bsd.pc++;
    }
    bsd.pc = 0;
    return 0;
}