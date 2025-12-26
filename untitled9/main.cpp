/* fopen guvenlik uyarisini susturmak icin */
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>   // Dosya islemleri (fopen) ve yazdirma (printf) icin
#include <stdlib.h>  // Hata durumunda programi kapatan exit komutu icin
#include <math.h>    // Standart sapma hesabindaki karekok (sqrt) icin
#include <string.h>  // Metin islemleri (strlen, memset) icin

// Ogrenci bilgilerini tutacak yapi
struct Ogrenci {
    char no[30];
    char ad[30];
    char soyad[30];
    double quiz;
    double standart_not;
};

// Fonksiyon bildirimleri (C dilinde dizi boyutu parametre olarak genelde verilir)
void ekleDegistir(struct Ogrenci ogr[], int kapasite);
void siralayarakYazdir(struct Ogrenci ogr[], int kapasite);

int main() {
    // 24 kisilik sabit bir dizi olusturuyoruz (Vector yerine Array)
    struct Ogrenci ogrenciler[24];

    // Dizinin icini temizliyoruz ki bos yerlerde rastgele veriler kalmasin
    memset(ogrenciler, 0, sizeof(ogrenciler));

    printf("Program baslatildi...\n");

    // Dosyadan oku ve hesapla
    ekleDegistir(ogrenciler, 24);

    // Sirala ve dosyaya yaz
    siralayarakYazdir(ogrenciler, 24);

    printf("Islem tamamlandi. Sonuclar 'stdquiz.txt' dosyasina yazildi.\n");

    return 0;
}

void ekleDegistir(struct Ogrenci ogr[], int kapasite) {
    FILE *dosya = fopen("quiz.txt", "r");

    if (dosya == NULL) {
        printf("HATA: 'quiz.txt' dosyasi acilamadi!\n");
        exit(1);
    }

    double toplamPuan = 0.0;
    double karelerToplami = 0.0;
    int n = 0;

    // Dosyadan okuma yapiyoruz (adres operatoru & dizilerde kullanilmaz, sadece sayilarda)
    while (n < kapasite && fscanf(dosya, "%s %s %s %lf",
                            ogr[n].no,
                            ogr[n].ad,
                            ogr[n].soyad,
                            &ogr[n].quiz) != EOF) {

        toplamPuan += ogr[n].quiz;
        karelerToplami += (ogr[n].quiz * ogr[n].quiz);
        n++;
    }
    fclose(dosya);

    if (n == 0) {
        printf("HATA: Dosyadan veri okunamadi.\n");
        exit(1);
    }

    // Matematiksel hesaplamalar
    double ortalama = toplamPuan / n;
    double pay = (n * karelerToplami) - (toplamPuan * toplamPuan);
    double payda = (double)n * (n - 1); // int bolmesi olmasin diye double yaptik

    if (pay < 0) pay = 0;

    double standartSapma = sqrt(pay / payda);

    // Z-Puani hesabi
    for (int i = 0; i < n; i++) {
        if (standartSapma != 0) {
            ogr[i].standart_not = (ogr[i].quiz - ortalama) / standartSapma;
        } else {
            ogr[i].standart_not = 0.0;
        }
    }
}

void siralayarakYazdir(struct Ogrenci ogr[], int kapasite) {
    // Kac ogrenci oldugunu buluyoruz
    int doluSayisi = 0;
    for(int i=0; i < kapasite; i++) {
        if(strlen(ogr[i].no) > 0) doluSayisi++;
    }

    // Bubble Sort (C'de swap fonksiyonu olmadigi icin elle yapiyoruz)
    for (int i = 0; i < doluSayisi - 1; i++) {
        for (int j = 0; j < doluSayisi - i - 1; j++) {
            // Eger simdiki, bir sonrakinden kucukse yer degistir
            if (ogr[j].standart_not < ogr[j + 1].standart_not) {
                struct Ogrenci gecici = ogr[j];
                ogr[j] = ogr[j + 1];
                ogr[j + 1] = gecici;
            }
        }
    }

    // Yazma islemi
    FILE *cikis = fopen("stdquiz.txt", "w");
    if (cikis == NULL) {
        printf("HATA: 'stdquiz.txt' dosyasi olusturulamadi.\n");
        exit(1);
    }

    // Basliklari yaz
    fprintf(cikis, "%-15s %-15s %-15s %-10s\n", "No", "Ad", "Soyad", "Z-Puan");
    fprintf(cikis, "-------------------------------------------------------\n");

    // Verileri yaz
    for (int i = 0; i < doluSayisi; i++) {
        fprintf(cikis, "%-15s %-15s %-15s %.2lf\n",
                ogr[i].no,
                ogr[i].ad,
                ogr[i].soyad,
                ogr[i].standart_not);
    }

    fclose(cikis);
}