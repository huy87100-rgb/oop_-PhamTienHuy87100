#include <iostream>
#include <string>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include <limits>

using namespace std;

class phuong_tien {
private:
    int ma_xe;
    string loai_xe;
    int gio_vao;
    int tg_do;

public:
    phuong_tien();
    phuong_tien(int ma, string loai, int gio, int tg);

    int lay_ma_xe() const;
    void dat_ma_xe(int ma);
    string lay_loai_xe() const;
    double tinh_phi() const;
    void hien_thi() const;
};

phuong_tien::phuong_tien() : ma_xe(0), loai_xe(""), gio_vao(0), tg_do(0) {}

phuong_tien::phuong_tien(int ma, string loai, int gio, int tg)
    : ma_xe(ma), loai_xe(loai), gio_vao(gio), tg_do(tg) {}

int phuong_tien::lay_ma_xe() const { return ma_xe; }
void phuong_tien::dat_ma_xe(int ma) { ma_xe = ma; }
string phuong_tien::lay_loai_xe() const { return loai_xe; }

double phuong_tien::tinh_phi() const {
    double gia = (loai_xe == "Car") ? 6.0 : (loai_xe == "Motorcycle") ? 4.0 : 2.0;
    bool cao_diem = (gio_vao >= 6 && gio_vao <= 9) || (gio_vao >= 15 && gio_vao <= 18);
    double tong = 0;
    for (int h = 0; h < tg_do; ++h) {
        if (h < 2) tong += gia * (cao_diem ? 1.30 : 0.95);
        else       tong += gia * 0.90;
    }
    return tong;
}

void phuong_tien::hien_thi() const {
    cout << "  ID: " << ma_xe << " | " << loai_xe
         << " | " << gio_vao << "h | " << tg_do << "h"
         << " | Phi: $" << fixed << setprecision(2) << tinh_phi() << "\n";
}

class bai_xe {
private:
    int ma_bai;
    char kich_thuoc;
    static const int MAX_VEHICLES = 3;
    phuong_tien danh_sach_pt[MAX_VEHICLES];
    int currentVehicleCount;

public:
    bai_xe();
    bai_xe(int ma, char kt);

    int lay_ma() const;
    void dat_ma(int ma);
    char lay_kich_thuoc() const;
    int lay_suc_chua() const;
    int lay_so_luong() const;
    bool isFull() const;

    int tim_index_xe(int ma_xe) const;
    bool them(phuong_tien v);
    bool xoa(int ma_xe);
    bool chua(int ma_xe) const;
    void hien_thi() const;
    bool co_loai(string loai, int& dem, double& tong) const;
};

bai_xe::bai_xe() : ma_bai(0), kich_thuoc('S'), currentVehicleCount(0) {}

bai_xe::bai_xe(int ma, char kt) : ma_bai(ma), kich_thuoc(kt), currentVehicleCount(0) {}

int bai_xe::lay_ma() const { return ma_bai; }
void bai_xe::dat_ma(int ma) { ma_bai = ma; }
char bai_xe::lay_kich_thuoc() const { return kich_thuoc; }

int bai_xe::lay_suc_chua() const {
    return (kich_thuoc == 'S') ? 1 : (kich_thuoc == 'M') ? 2 : 3;
}

int bai_xe::lay_so_luong() const {
    return currentVehicleCount;
}

bool bai_xe::isFull() const {
    return currentVehicleCount >= lay_suc_chua();
}

int bai_xe::tim_index_xe(int ma_xe) const {
    for (int i = 0; i < currentVehicleCount; ++i) {
        if (danh_sach_pt[i].lay_ma_xe() == ma_xe) {
            return i;
        }
    }
    return -1;
}

bool bai_xe::them(phuong_tien v) {
    if (isFull()) return false;
    danh_sach_pt[currentVehicleCount] = v;
    currentVehicleCount++;
    return true;
}

bool bai_xe::xoa(int ma_xe) {
    int idx = tim_index_xe(ma_xe);
    if (idx == -1) return false;

    for (int i = idx; i < currentVehicleCount - 1; ++i) {
        danh_sach_pt[i] = danh_sach_pt[i + 1];
    }
    currentVehicleCount--;
    return true;
}

bool bai_xe::chua(int ma_xe) const {
    return tim_index_xe(ma_xe) != -1;
}

void bai_xe::hien_thi() const {
    cout << "Bai " << ma_bai << " [" << kich_thuoc << "] "
         << currentVehicleCount << "/" << lay_suc_chua() << " xe\n";
    for (int i = 0; i < currentVehicleCount; ++i) {
        danh_sach_pt[i].hien_thi();
    }
}

bool bai_xe::co_loai(string loai, int& dem, double& tong) const {
    dem = 0; tong = 0;
    for (int i = 0; i < currentVehicleCount; ++i) {
        if (danh_sach_pt[i].lay_loai_xe() == loai) {
            ++dem;
            tong += danh_sach_pt[i].tinh_phi();
        }
    }
    return dem > 0;
}

class SystemInterface {
private:
    static const int MAX_LOTS = 10;
    bai_xe parkingLots[MAX_LOTS];
    int currentLotCount;

    int nhap_so_nguyen(const string& thong_bao);
    int generateRandomID();
    bool isParkIDExists(int pID) const;
    bool isVehicleIDExists(int vID) const;

public:
    SystemInterface();
    void runMenu();
};

SystemInterface::SystemInterface() : currentLotCount(0) {
    srand(time(0));
}

int SystemInterface::nhap_so_nguyen(const string& thong_bao) {
    int val;
    while (true) {
        cout << thong_bao;
        if (cin >> val) return val;
        cout << "Loi! Vui long nhap mot so nguyen hop le.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

int SystemInterface::generateRandomID() {
    return rand() % 9000 + 1000;
}

bool SystemInterface::isParkIDExists(int pID) const {
    for (int i = 0; i < currentLotCount; ++i) {
        if (parkingLots[i].lay_ma() == pID) return true;
    }
    return false;
}

bool SystemInterface::isVehicleIDExists(int vID) const {
    for (int i = 0; i < currentLotCount; ++i) {
        if (parkingLots[i].chua(vID)) return true;
    }
    return false;
}

void SystemInterface::runMenu() {
    int chon;
    while (true) {
        cout << "\n===== QUAN LY BAI DO XE =====\n";
        cout << "1. Tao bai do\n";
        cout << "2. Them xe\n";
        cout << "3. Xoa bai do\n";
        cout << "4. Lay xe ra\n";
        cout << "5. Hien thi tat ca bai do\n";
        cout << "6. Hien thi xe theo bai\n";
        cout << "7. Liet ke xe theo loai\n";
        cout << "8. Bao cao phi trung binh\n";
        cout << "9. Thoat\n";
        
        chon = nhap_so_nguyen("Chon: ");

        switch (chon) {
            case 1: {
                if (currentLotCount >= MAX_LOTS) {
                    cout << "Canh bao: He thong da dat toi da (10 bai do)!\n";
                    break;
                }

                int ma = nhap_so_nguyen("Ma bai: ");
                char kt;
                cout << "Kich thuoc (S/M/L): ";
                cin >> kt;
                kt = toupper(kt);
                while (kt != 'S' && kt != 'M' && kt != 'L') {
                    cout << "Kich thuoc (S/M/L): ";
                    cin >> kt;
                    kt = toupper(kt);
                }

                if (isParkIDExists(ma)) {
                    int id_moi = generateRandomID();
                    while (isParkIDExists(id_moi)) id_moi = generateRandomID();
                    cout << "Canh bao: Ma bai " << ma << " da ton tai! Tu dong gan ID: " << id_moi << "\n";
                    ma = id_moi;
                }

                parkingLots[currentLotCount] = bai_xe(ma, kt);
                currentLotCount++;
                cout << "Da tao bai " << ma << "\n";
                break;
            }

            case 2: {
                if (currentLotCount == 0) {
                    cout << "Chua co bai do nao!\n";
                    break;
                }

                int ma = nhap_so_nguyen("Ma xe: ");
                string loai;
                cout << "Loai (Car/Motorcycle/Bicycle): ";
                cin >> loai;

                int gio = nhap_so_nguyen("Gio vao (0-23): ");
                while (gio < 0 || gio > 23) {
                    gio = nhap_so_nguyen("Gio vao (0-23): ");
                }

                int tg = nhap_so_nguyen("Thoi luong (1-23): ");
                while (tg < 1 || tg > 23) {
                    tg = nhap_so_nguyen("Thoi luong (1-23): ");
                }

                if (isVehicleIDExists(ma)) {
                    int id_moi = generateRandomID();
                    while (isVehicleIDExists(id_moi)) id_moi = generateRandomID();
                    cout << "Canh bao: Ma xe " << ma << " da ton tai! Tu dong gan ID: " << id_moi << "\n";
                    ma = id_moi;
                }

                phuong_tien v(ma, loai, gio, tg);
                bool ok = false;

                for (int i = 0; i < currentLotCount; ++i) {
                    if (!parkingLots[i].isFull()) {
                        ok = parkingLots[i].them(v);
                        if (ok) {
                            cout << "Da them xe " << ma << " | Phi: $" << fixed << setprecision(2) << v.tinh_phi() << "\n";
                            break;
                        }
                    }
                }

                if (!ok) {
                    cout << "Tat ca bai da day!\n";
                }
                break;
            }

            case 3: {
                int ma = nhap_so_nguyen("Ma bai can xoa: ");
                int idx = -1;
                for (int i = 0; i < currentLotCount; ++i) {
                    if (parkingLots[i].lay_ma() == ma) {
                        idx = i;
                        break;
                    }
                }

                if (idx != -1) {
                    for (int i = idx; i < currentLotCount - 1; ++i) {
                        parkingLots[i] = parkingLots[i + 1];
                    }
                    currentLotCount--;
                    cout << "Da xoa bai " << ma << "\n";
                }
                break;
            }

            case 4: {
                int ma = nhap_so_nguyen("Ma xe can lay ra: ");
                bool ok = false;
                for (int i = 0; i < currentLotCount; ++i) {
                    if (parkingLots[i].xoa(ma)) {
                        ok = true;
                        break;
                    }
                }
                cout << (ok ? "Da lay xe ra\n" : "Khong tim thay xe\n");
                break;
            }

            case 5: {
                for (int i = 0; i < currentLotCount; ++i) {
                    parkingLots[i].hien_thi();
                }
                break;
            }

            case 6: {
                int ma = nhap_so_nguyen("Ma bai: ");
                for (int i = 0; i < currentLotCount; ++i) {
                    if (parkingLots[i].lay_ma() == ma) {
                        parkingLots[i].hien_thi();
                        break;
                    }
                }
                break;
            }

            case 7: {
                string loai;
                cout << "Loai xe: ";
                cin >> loai;

                int dem;
                double tong;
                for (int i = 0; i < currentLotCount; ++i) {
                    if (parkingLots[i].co_loai(loai, dem, tong)) {
                        parkingLots[i].hien_thi();
                    }
                }
                break;
            }

            case 8: {
                string loai_arr[3] = {"Car", "Motorcycle", "Bicycle"};
                for (int t = 0; t < 3; ++t) {
                    int dem_tong = 0;
                    double tong_phi_all = 0;
                    int d;
                    double s;

                    for (int i = 0; i < currentLotCount; ++i) {
                        if (parkingLots[i].co_loai(loai_arr[t], d, s)) {
                            dem_tong += d;
                            tong_phi_all += s;
                        }
                    }

                    cout << loai_arr[t] << ": ";
                    if (dem_tong == 0) cout << "khong co xe\n";
                    else cout << dem_tong << " xe | TB: $" << fixed << setprecision(2) << tong_phi_all / dem_tong << "\n";
                }
                break;
            }

            case 9:
                return;

            default:
                break;
        }
    }
}

int main() {
    SystemInterface sys;
    sys.runMenu();
    return 0;
}
