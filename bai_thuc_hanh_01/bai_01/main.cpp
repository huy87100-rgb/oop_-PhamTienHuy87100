#include <iostream>
#include <string>
#include <cctype>
#include <iomanip>

using namespace std;

class phuong_tien {
private:
    int ma_xe;
    string loai_xe;
    int gio_vao;
    int tg_do;

public:
    phuong_tien() : ma_xe(0), loai_xe(""), gio_vao(0), tg_do(0) {}

    phuong_tien(int ma, string loai, int gio, int tg) 
        : ma_xe(ma), gio_vao(gio), tg_do(tg) {
        string s = "";
        for (char c : loai) s += tolower(c);

        if (s == "car" || s == "xe o to" || s == "oto") loai_xe = "Xe o to";
        else if (s == "motorcycle" || s == "xe may") loai_xe = "Xe may";
        else if (s == "bicycle" || s == "xe dap") loai_xe = "Xe dap";
        else loai_xe = "Khong xac dinh";
    }

    int lay_ma_xe() const { return ma_xe; }
    string lay_loai_xe() const { return loai_xe; }

    double tinh_phi() const {
        double gia_co_ban = 0.0;
        if (loai_xe == "Xe o to") gia_co_ban = 6.0;
        else if (loai_xe == "Xe may") gia_co_ban = 4.0;
        else if (loai_xe == "Xe dap") gia_co_ban = 2.0;

        bool cao_diem = (gio_vao >= 6 && gio_vao < 10) || (gio_vao >= 15 && gio_vao < 19);

        double tong = 0.0;
        for (int h = 1; h <= tg_do; ++h) {
            if (h <= 2) {
                if (cao_diem) tong += gia_co_ban * 1.30;
                else tong += gia_co_ban * 0.95;
            } else {
                tong += gia_co_ban * 0.90;
            }
        }
        return tong;
    }

    void hien_thi() const {
        if (ma_xe == 0) return;
        cout << "  - Ma xe: " << ma_xe 
             << " | Loai: " << loai_xe 
             << " | Gio vao: " << gio_vao << "h"
             << " | Thoi gian do: " << tg_do << "h"
             << " | Phi do: $" << fixed << setprecision(2) << tinh_phi() << "\n";
    }
};

class bai_xe {
private:
    int ma_bai;
    char kich_thuoc;
    phuong_tien pt1, pt2, pt3;
    bool co1, co2, co3;

public:
    bai_xe() : ma_bai(0), kich_thuoc('S'), co1(false), co2(false), co3(false) {}

    bai_xe(int ma, char kt) : ma_bai(ma), kich_thuoc(toupper(kt)), co1(false), co2(false), co3(false) {}

    int lay_ma() const { return ma_bai; }

    int lay_suc_chua() const {
        if (kich_thuoc == 'S') return 1;
        if (kich_thuoc == 'M') return 2;
        if (kich_thuoc == 'L') return 3;
        return 0;
    }

    int lay_so_luong() const {
        int dem = 0;
        if (co1) dem++;
        if (co2) dem++;
        if (co3) dem++;
        return dem;
    }

    bool them(phuong_tien v) {
        int cap = lay_suc_chua();
        if (lay_so_luong() >= cap) return false;

        if (!co1 && cap >= 1) { pt1 = v; co1 = true; return true; }
        if (!co2 && cap >= 2) { pt2 = v; co2 = true; return true; }
        if (!co3 && cap >= 3) { pt3 = v; co3 = true; return true; }
        return false;
    }

    bool xoa(int ma_xe) {
        if (co1 && pt1.lay_ma_xe() == ma_xe) { co1 = false; return true; }
        if (co2 && pt2.lay_ma_xe() == ma_xe) { co2 = false; return true; }
        if (co3 && pt3.lay_ma_xe() == ma_xe) { co3 = false; return true; }
        return false;
    }

    bool chua(int ma_xe) const {
        if (co1 && pt1.lay_ma_xe() == ma_xe) return true;
        if (co2 && pt2.lay_ma_xe() == ma_xe) return true;
        if (co3 && pt3.lay_ma_xe() == ma_xe) return true;
        return false;
    }

    void hien_thi() const {
        cout << "\n----------------------------------------\n";
        cout << "Ma bai do: " << ma_bai 
             << " | Kich thuoc: " << kich_thuoc 
             << " | So xe: " << lay_so_luong() << "/" << lay_suc_chua() << "\n";
        if (co1) pt1.hien_thi();
        if (co2) pt2.hien_thi();
        if (co3) pt3.hien_thi();
    }

    bool co_loai(string loai, int& dem, double& tong) const {
        string target = "";
        for (char c : loai) target += tolower(c);
        if (target == "car" || target == "oto") target = "xe o to";
        if (target == "motorcycle") target = "xe may";
        if (target == "bicycle") target = "xe dap";

        bool tim_thay = false;

        auto kiem_tra = [&](const phuong_tien& pt, bool co) {
            if (co) {
                string l = pt.lay_loai_xe();
                for (char& c : l) c = tolower(c);
                if (l == target) {
                    dem++;
                    tong += pt.tinh_phi();
                    cout << "  [Bai ID: " << ma_bai << "] ";
                    pt.hien_thi();
                    tim_thay = true;
                }
            }
        };

        kiem_tra(pt1, co1);
        kiem_tra(pt2, co2);
        kiem_tra(pt3, co3);

        return tim_thay;
    }

    void thong_ke_bai() const {
        int dC = 0, dM = 0, dB = 0;
        double fC = 0.0, fM = 0.0, fB = 0.0;

        auto tinh_pt = [&](const phuong_tien& pt) {
            if (pt.lay_loai_xe() == "Xe o to") { dC++; fC += pt.tinh_phi(); }
            else if (pt.lay_loai_xe() == "Xe may") { dM++; fM += pt.tinh_phi(); }
            else if (pt.lay_loai_xe() == "Xe dap") { dB++; fB += pt.tinh_phi(); }
        };

        if (co1) tinh_pt(pt1);
        if (co2) tinh_pt(pt2);
        if (co3) tinh_pt(pt3);

        cout << "  > Bai " << ma_bai << " (Kich thuoc " << kich_thuoc << "):\n";
        cout << "    - Xe o to: " << dC << " xe | Trung binh: $" << (dC > 0 ? fC / dC : 0.0) << "\n";
        cout << "    - Xe may: " << dM << " xe | Trung binh: $" << (dM > 0 ? fM / dM : 0.0) << "\n";
        cout << "    - Xe dap: " << dB << " xe | Trung binh: $" << (dB > 0 ? fB / dB : 0.0) << "\n";
    }

    void gom_tong(int& dC, double& fC, int& dM, double& fM, int& dB, double& fB) const {
        auto tinh_pt = [&](const phuong_tien& pt) {
            if (pt.lay_loai_xe() == "Xe o to") { dC++; fC += pt.tinh_phi(); }
            else if (pt.lay_loai_xe() == "Xe may") { dM++; fM += pt.tinh_phi(); }
            else if (pt.lay_loai_xe() == "Xe dap") { dB++; fB += pt.tinh_phi(); }
        };

        if (co1) tinh_pt(pt1);
        if (co2) tinh_pt(pt2);
        if (co3) tinh_pt(pt3);
    }
};

class GiaoDienHeThong {
private:
    bai_xe bai1, bai2, bai3;
    bool co1, co2, co3;

    bai_xe* tim_bai(int ma) {
        if (co1 && bai1.lay_ma() == ma) return &bai1;
        if (co2 && bai2.lay_ma() == ma) return &bai2;
        if (co3 && bai3.lay_ma() == ma) return &bai3;
        return nullptr;
    }

public:
    GiaoDienHeThong() : co1(false), co2(false), co3(false) {}

    void chay_danh_muc() {
        int chon = 0;
        do {
            cout << "\n========================================\n";
            cout << "    HE THONG QUAN LY BAI DO XE\n";
            cout << "========================================\n";
            cout << "1. Tao bai do xe moi\n";
            cout << "2. Them xe vao bai do\n";
            cout << "3. Xoa mot bai do xe\n";
            cout << "4. Lay xe ra khoi bai do\n";
            cout << "5. Hien thi tat ca bai do xe\n";
            cout << "6. Hien thi danh sach xe theo bai do\n";
            cout << "7. Liet ke xe theo loai phuong tien\n";
            cout << "8. Xem bao cao phi do xe trung binh\n";
            cout << "9. Thoat\n";
            cout << "Nhap lua chon (1-9): ";
            cin >> chon;

            switch (chon) {
                case 1: tao_bai(); break;
                case 2: them_xe(); break;
                case 3: xoa_bai(); break;
                case 4: lay_xe(); break;
                case 5: hien_thi_tat_ca(); break;
                case 6: hien_thi_theo_bai(); break;
                case 7: liet_ke_theo_loai(); break;
                case 8: xem_bao_cao(); break;
                case 9: cout << "Tam biet!\n"; break;
                default: cout << "Lua chon khong le!\n";
            }
        } while (chon != 9);
    }

private:
    void tao_bai() {
        if (co1 && co2 && co3) {
            cout << "He thong da toi da 3 bai do!\n";
            return;
        }

        int ma;
        char kt;
        cout << "Nhap ma bai do: ";
        cin >> ma;

        if (tim_bai(ma) != nullptr) {
            cout << "Ma bai do nay da ton tai!\n";
            return;
        }

        cout << "Nhap kich thuoc (S/M/L): ";
        cin >> kt;
        kt = toupper(kt);

        if (kt != 'S' && kt != 'M' && kt != 'L') {
            cout << "Kich thuoc chi duoc nhap S, M hoac L!\n";
            return;
        }

        if (!co1) { bai1 = bai_xe(ma, kt); co1 = true; }
        else if (!co2) { bai2 = bai_xe(ma, kt); co2 = true; }
        else if (!co3) { bai3 = bai_xe(ma, kt); co3 = true; }
        cout << "Tao bai do xe thanh cong!\n";
    }

    void them_xe() {
        int ma_xe, gio, tg;
        string loai;

        cout << "Nhap ma xe: ";
        cin >> ma_xe;

        if ((co1 && bai1.chua(ma_xe)) ||
            (co2 && bai2.chua(ma_xe)) ||
            (co3 && bai3.chua(ma_xe))) {
            cout << "Ma xe nay da co trong he thong!\n";
            return;
        }

        cout << "Nhap loai xe (Xe o to / Xe may / Xe dap): ";
        cin.ignore();
        getline(cin, loai);
        cout << "Nhap gio vao (0-23): ";
        cin >> gio;
        cout << "Nhap thoi gian do (1-23): ";
        cin >> tg;

        phuong_tien pt(ma_xe, loai, gio, tg);

        if (co1 && bai1.them(pt)) { cout << "Da xep xe vao bai ID: " << bai1.lay_ma() << "\n"; return; }
        if (co2 && bai2.them(pt)) { cout << "Da xep xe vao bai ID: " << bai2.lay_ma() << "\n"; return; }
        if (co3 && bai3.them(pt)) { cout << "Da xep xe vao bai ID: " << bai3.lay_ma() << "\n"; return; }

        cout << "Khong tim duoc bai do con trong hoac phu hop!\n";
    }

    void xoa_bai() {
        int ma;
        cout << "Nhap ma bai do can xoa: ";
        cin >> ma;

        if (co1 && bai1.lay_ma() == ma) { co1 = false; cout << "Da xoa bai do ID " << ma << "\n"; }
        else if (co2 && bai2.lay_ma() == ma) { co2 = false; cout << "Da xoa bai do ID " << ma << "\n"; }
        else if (co3 && bai3.lay_ma() == ma) { co3 = false; cout << "Da xoa bai do ID " << ma << "\n"; }
        else { cout << "Khong tim thay ma bai do nay!\n"; }
    }

    void lay_xe() {
        int ma_xe;
        cout << "Nhap ma xe can lay ra: ";
        cin >> ma_xe;

        if (co1 && bai1.xoa(ma_xe)) { cout << "Da lay xe ID " << ma_xe << " ra khoi bai ID " << bai1.lay_ma() << "\n"; return; }
        if (co2 && bai2.xoa(ma_xe)) { cout << "Da lay xe ID " << ma_xe << " ra khoi bai ID " << bai2.lay_ma() << "\n"; return; }
        if (co3 && bai3.xoa(ma_xe)) { cout << "Da lay xe ID " << ma_xe << " ra khoi bai ID " << bai3.lay_ma() << "\n"; return; }

        cout << "Khong tim thay ma xe nay!\n";
    }

    void hien_thi_tat_ca() const {
        if (!co1 && !co2 && !co3) {
            cout << "Chua co bai do xe nao duoc tao!\n";
            return;
        }
        if (co1) bai1.hien_thi();
        if (co2) bai2.hien_thi();
        if (co3) bai3.hien_thi();
    }

    void hien_thi_theo_bai() {
        int ma;
        cout << "Nhap ma bai do: ";
        cin >> ma;

        bai_xe* b = tim_bai(ma);
        if (b != nullptr) {
            b->hien_thi();
        } else {
            cout << "Khong tim thay bai do xe!\n";
        }
    }

    void liet_ke_theo_loai() const {
        string loai;
        cout << "Nhap loai xe (Xe o to / Xe may / Xe dap): ";
        cin.ignore();
        getline(cin, loai);

        cout << "\n--- DANH SACH XE THEO LOAI: " << loai << " ---\n";
        int d = 0; double t = 0.0;
        if (co1) bai1.co_loai(loai, d, t);
        if (co2) bai2.co_loai(loai, d, t);
        if (co3) bai3.co_loai(loai, d, t);
    }

    void xem_bao_cao() const {
        int tongCar = 0, tongMoto = 0, tongBike = 0;
        double tongPhiCar = 0.0, tongPhiMoto = 0.0, tongPhiBike = 0.0;

        if (co1) bai1.gom_tong(tongCar, tongPhiCar, tongMoto, tongPhiMoto, tongBike, tongPhiBike);
        if (co2) bai2.gom_tong(tongCar, tongPhiCar, tongMoto, tongPhiMoto, tongBike, tongPhiBike);
        if (co3) bai3.gom_tong(tongCar, tongPhiCar, tongMoto, tongPhiMoto, tongBike, tongPhiBike);

        cout << "\n========================================\n";
        cout << "        BAO CAO THONG KE PHI DO XE\n";
        cout << "========================================\n";
        cout << "1. PHI TRUNG BINH TOAN HE THONG:\n";
        cout << "  - Xe o to: $" << fixed << setprecision(2) << (tongCar > 0 ? tongPhiCar / tongCar : 0.0) << " (Tong: " << tongCar << " xe)\n";
        cout << "  - Xe may: $" << (tongMoto > 0 ? tongPhiMoto / tongMoto : 0.0) << " (Tong: " << tongMoto << " xe)\n";
        cout << "  - Xe dap: $" << (tongBike > 0 ? tongPhiBike / tongBike : 0.0) << " (Tong: " << tongBike << " xe)\n";

        cout << "\n2. CHI TIET THEO TUNG BAI DO XE:\n";
        if (co1) bai1.thong_ke_bai();
        if (co2) bai2.thong_ke_bai();
        if (co3) bai3.thong_ke_bai();
    }
};

int main() {
    GiaoDienHeThong ungDung;
    ungDung.chay_danh_muc();
    return 0;
}
