#include <iostream>
#include <iomanip>
#include <cstring>
#include <stdlib.h>
#include <fstream>
#include <string>
using namespace std;

struct SuStok {
    char musteriAdi[50];
    char musteriSoyadi[50];
    int musteriNumarasi;
    int stokMiktari;  
    char adres[100];
};

void StokEkle(SuStok stok) {
    ofstream Myfile("SuStok.dat", ios::app | ios::binary);

    cout << "Müþteri adýný giriniz: ";
 
    cin >> stok.musteriAdi;

    cout << "Müþteri soyadýný giriniz: ";
    cin >> stok.musteriSoyadi ;

    cout << "Müþteri numarasýný giriniz: ";
    cin >> stok.musteriNumarasi;
     

    cout << "Stok miktarýný (litre) giriniz: ";
    cin >> stok.stokMiktari;
     

    cout << "Müþteri adresini giriniz: ";
    cin >> stok.adres, 100 ;

    Myfile.write(reinterpret_cast<char*>(&stok), sizeof(stok));
    Myfile.close();

    cout << "Stok baþarýyla eklendi!\n";
}

void StokListele(SuStok stok, int kayitSayisi) {
    ifstream Myfile("SuStok.dat", ios::binary);
    cout << "\nKayýtlarda toplam müþteri sayýsý: " << kayitSayisi << endl;

    if (kayitSayisi > 0) {
        for (int i = 0; i < kayitSayisi; i++) {
            Myfile.seekg(i * sizeof(stok));
            Myfile.read(reinterpret_cast<char*>(&stok), sizeof(stok));

            cout << "\n" << i + 1 << ". Müþteri\n";
            cout << "Adý: " << stok.musteriAdi << endl;
            cout << "Soyadý: " << stok.musteriSoyadi << endl;
            cout << "Numarasý: " << stok.musteriNumarasi << endl;
            cout << "Stok Miktarý (Litre): " << stok.stokMiktari << endl;
            cout << "Adres: " << stok.adres << endl;
        }
    }
    else {
        cout << "Kayýt Yok.";
    }

    Myfile.close();
}

void StokGuncelle(int kayitSayisi, SuStok stok) {
    int numara;
    bool kayitBulundu = false;

    fstream Myfile("SuStok.dat", ios::in | ios::out | ios::binary);
    if (!Myfile.is_open()) {
        cerr << "Dosya açýlamadý!" << endl;
        return;
    }

    cout << "Düzenlemek istediðiniz müþterinin numarasýný giriniz: ";
    cin >> numara;

    for (int i = 0; i < kayitSayisi; i++) {
        Myfile.seekg(i * sizeof(stok));
        Myfile.read(reinterpret_cast<char*>(&stok), sizeof(stok));

        if (stok.musteriNumarasi == numara) {
            kayitBulundu = true;
            cout << "\nKayýt bulundu:\n";
            cout << "Adý: " << stok.musteriAdi << endl;
            cout << "Soyadý: " << stok.musteriSoyadi << endl;
            cout << "Numarasý: " << stok.musteriNumarasi << endl;
            cout << "Stok Miktarý (Litre): " << stok.stokMiktari << endl;
            cout << "Adres: " << stok.adres << endl;

            cout << "Yeni stok miktarýný (litre) giriniz: ";
            cin >> stok.stokMiktari;

            Myfile.seekp(i * sizeof(stok));
            Myfile.write(reinterpret_cast<char*>(&stok), sizeof(stok));
            cout << "Stok baþarýyla güncellendi!\n";
            break;
        }
    }

    if (!kayitBulundu) {
        cout << "Girilen numaraya ait kayýt bulunamadý." << endl;
    }

    Myfile.close();
}

void StokSil(int kayitSayisi, SuStok stok) {
    int numara;
    bool kayitBulundu = false;

    ifstream Myfile("SuStok.dat", ios::binary);
    ofstream YedekFile("Yedek.dat", ios::binary);

    if (!Myfile.is_open() || !YedekFile.is_open()) {
        cerr << "Dosya açýlamadý!" << endl;
        return;
    }

    cout << "Silmek istediðiniz müþterinin numarasýný giriniz: ";
    cin >> numara;

    for (int i = 0; i < kayitSayisi; i++) {
        Myfile.read(reinterpret_cast<char*>(&stok), sizeof(stok));

        if (stok.musteriNumarasi == numara) {
            kayitBulundu = true;
            cout << "\nKayýt bulundu ve silindi.\n";
            continue;
        }

        YedekFile.write(reinterpret_cast<char*>(&stok), sizeof(stok));
    }

    if (!kayitBulundu) {
        cout << "Girilen numaraya ait kayýt bulunamadý." << endl;
    }

    Myfile.close();
    YedekFile.close();

    remove("SuStok.dat");
    rename("Yedek.dat", "SuStok.dat");
}

int main() {
    setlocale(LC_ALL, "Turkish");
    SuStok stok{};
    int karar;
    char devam;

    ifstream Myfile("SuStok.dat", ios::binary);
    if (!Myfile.is_open()) {
        ofstream Myfile("SuStok.dat");
    }

    Myfile.seekg(0, ios::end);
    int kayitSayisi = Myfile.tellg() / sizeof(stok);
    Myfile.close();

    do {
        cout << "1. Stok Ekle\n2. Stok Listele\n3. Stok Güncelle\n4. Stok Sil\n (Lütfen Bilgileri Boþluk býrakmadan giriniz!!) \n" ;
        cout << "Yapmak istediðiniz iþlemin sýra numarasýný giriniz: ";
        cin >> karar;

        switch (karar) {
        case 1:
            StokEkle(stok);
            kayitSayisi++;
            break;
        case 2:
            StokListele(stok, kayitSayisi);
            break;
        case 3:
            StokGuncelle(kayitSayisi, stok);
            break;
        case 4:
            StokSil(kayitSayisi, stok);
            kayitSayisi--;
            break;
        default:
            cout << "Geçersiz seçim." << endl;
        }

        cout << "\nBaþka bir iþlem yapmak ister misiniz? (E/H): ";
        cin >> devam;

    } while (devam == 'E' || devam == 'e');

    return 0;
}