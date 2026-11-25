#include <iostream>
#include <string>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include <limits>
#include <cctype>

using namespace std;

class Vehicle {
private:
    int vehicleID;
    string vehicleType;
    int entryTime;
    int parkingDuration;
    double parkingFee;

public:
    Vehicle();
    Vehicle(int id, const string& type, int entry, int duration);

    int getVehicleID() const;
    void setVehicleID(int id);

    string getVehicleType() const;
    void setVehicleType(const string& type);

    int getEntryTime() const;
    void setEntryTime(int entry);

    int getParkingDuration() const;
    void setParkingDuration(int duration);

    double getParkingFee() const;
    double calculateFee() const;
    void display() const;
};

Vehicle::Vehicle()
    : vehicleID(0), vehicleType(""), entryTime(0), parkingDuration(0), parkingFee(0.0) {}

Vehicle::Vehicle(int id, const string& type, int entry, int duration)
    : vehicleID(id), vehicleType(type), entryTime(entry), parkingDuration(duration), parkingFee(0.0) {
    parkingFee = calculateFee();
}

int Vehicle::getVehicleID() const { return vehicleID; }
void Vehicle::setVehicleID(int id) { vehicleID = id; }

string Vehicle::getVehicleType() const { return vehicleType; }
void Vehicle::setVehicleType(const string& type) {
    vehicleType = type;
    parkingFee = calculateFee();
}

int Vehicle::getEntryTime() const { return entryTime; }
void Vehicle::setEntryTime(int entry) {
    entryTime = entry;
    parkingFee = calculateFee();
}

int Vehicle::getParkingDuration() const { return parkingDuration; }
void Vehicle::setParkingDuration(int duration) {
    parkingDuration = duration;
    parkingFee = calculateFee();
}

double Vehicle::getParkingFee() const {
    return calculateFee();
}

double Vehicle::calculateFee() const {
    double gia = (vehicleType == "Car") ? 6.0 : (vehicleType == "Motorcycle") ? 4.0 : 2.0;
    bool cao_diem = (entryTime >= 6 && entryTime <= 9) || (entryTime >= 15 && entryTime <= 18);
    double tong = 0.0;
    for (int h = 0; h < parkingDuration; ++h) {
        if (h < 2) tong += gia * (cao_diem ? 1.30 : 0.95);
        else       tong += gia * 0.90;
    }
    return tong;
}

void Vehicle::display() const {
    cout << "  ID: " << vehicleID << " | " << vehicleType
         << " | " << entryTime << "h | " << parkingDuration << "h"
         << " | Phi: $" << fixed << setprecision(2) << calculateFee() << "\n";
}


class ParkingLot {
private:
    int parkID;
    char size;
    int capacity;
    int currentVehicleCount;
    Vehicle** vehicles;

    int initialCapacityBySize(char s) const {
        char up = static_cast<char>(toupper(static_cast<unsigned char>(s)));
        return (up == 'S') ? 1 : (up == 'M') ? 2 : 3;
    }

public:
    ParkingLot();
    ParkingLot(int id, char s);
    ~ParkingLot();
    ParkingLot(const ParkingLot& other);
    ParkingLot& operator=(const ParkingLot& other);

    int getParkID() const;
    void setParkID(int id);
    char getSize() const;
    int getCapacity() const;
    int getCurrentVehicleCount() const;

    void resize(int newCapacity);
    bool addVehicle(const Vehicle& v);
    bool removeVehicle(int vID);
    int findVehicleIndex(int vID) const;
    bool containsVehicle(int vID) const;
    void display() const;
    bool hasVehicleType(const string& type, int& count, double& totalFee) const;
};

ParkingLot::ParkingLot()
    : parkID(0), size('S'), capacity(1), currentVehicleCount(0), vehicles(nullptr) {
    vehicles = new Vehicle*[capacity];
    for (int i = 0; i < capacity; ++i) {
        vehicles[i] = nullptr;
    }
}

ParkingLot::ParkingLot(int id, char s)
    : parkID(id),
      size(static_cast<char>(toupper(static_cast<unsigned char>(s)))),
      capacity(initialCapacityBySize(s)),
      currentVehicleCount(0),
      vehicles(nullptr) {
    vehicles = new Vehicle*[capacity];
    for (int i = 0; i < capacity; ++i) {
        vehicles[i] = nullptr;
    }
}

ParkingLot::~ParkingLot() {
    if (vehicles != nullptr) {
        for (int i = 0; i < currentVehicleCount; ++i) {
            delete vehicles[i];
            vehicles[i] = nullptr;
        }
        delete[] vehicles;
        vehicles = nullptr;
    }
    currentVehicleCount = 0;
    capacity = 0;
}

ParkingLot::ParkingLot(const ParkingLot& other)
    : parkID(other.parkID),
      size(other.size),
      capacity(other.capacity),
      currentVehicleCount(other.currentVehicleCount),
      vehicles(nullptr) {
    vehicles = new Vehicle*[capacity];
    for (int i = 0; i < capacity; ++i) {
        vehicles[i] = nullptr;
    }
    for (int i = 0; i < currentVehicleCount; ++i) {
        if (other.vehicles[i] != nullptr) {
            vehicles[i] = new Vehicle(*other.vehicles[i]);
        }
    }
}

ParkingLot& ParkingLot::operator=(const ParkingLot& other) {
    if (this == &other) return *this;

    if (vehicles != nullptr) {
        for (int i = 0; i < currentVehicleCount; ++i) {
            delete vehicles[i];
            vehicles[i] = nullptr;
        }
        delete[] vehicles;
        vehicles = nullptr;
    }

    parkID = other.parkID;
    size = other.size;
    capacity = other.capacity;
    currentVehicleCount = other.currentVehicleCount;

    vehicles = new Vehicle*[capacity];
    for (int i = 0; i < capacity; ++i) {
        vehicles[i] = nullptr;
    }
    for (int i = 0; i < currentVehicleCount; ++i) {
        if (other.vehicles[i] != nullptr) {
            vehicles[i] = new Vehicle(*other.vehicles[i]);
        }
    }

    return *this;
}

int ParkingLot::getParkID() const { return parkID; }
void ParkingLot::setParkID(int id) { parkID = id; }
char ParkingLot::getSize() const { return size; }
int ParkingLot::getCapacity() const { return capacity; }
int ParkingLot::getCurrentVehicleCount() const { return currentVehicleCount; }

void ParkingLot::resize(int newCapacity) {
    if (newCapacity <= 0) newCapacity = 1;
    if (newCapacity < currentVehicleCount) newCapacity = currentVehicleCount;

    Vehicle** newArr = new Vehicle*[newCapacity];
    for (int i = 0; i < newCapacity; ++i) {
        newArr[i] = nullptr;
    }

    for (int i = 0; i < currentVehicleCount; ++i) {
        newArr[i] = vehicles[i];
        if (vehicles != nullptr) {
            vehicles[i] = nullptr;
        }
    }

    delete[] vehicles;
    vehicles = newArr;
    capacity = newCapacity;
}

bool ParkingLot::addVehicle(const Vehicle& v) {
    if (currentVehicleCount >= capacity) {
        int nextCap = (capacity > 0) ? capacity * 2 : 1;
        resize(nextCap);
    }

    vehicles[currentVehicleCount] = new Vehicle(v);
    currentVehicleCount++;
    return true;
}

int ParkingLot::findVehicleIndex(int vID) const {
    if (vehicles == nullptr) return -1;
    for (int i = 0; i < currentVehicleCount; ++i) {
        if (vehicles[i] != nullptr && vehicles[i]->getVehicleID() == vID) {
            return i;
        }
    }
    return -1;
}

bool ParkingLot::removeVehicle(int vID) {
    int idx = findVehicleIndex(vID);
    if (idx == -1) return false;

    delete vehicles[idx];
    vehicles[idx] = nullptr;

    for (int i = idx; i < currentVehicleCount - 1; ++i) {
        vehicles[i] = vehicles[i + 1];
    }

    vehicles[currentVehicleCount - 1] = nullptr;
    currentVehicleCount--;
    return true;
}

bool ParkingLot::containsVehicle(int vID) const {
    return findVehicleIndex(vID) != -1;
}

void ParkingLot::display() const {
    cout << "Bai " << parkID << " [" << size << "] "
         << currentVehicleCount << "/" << capacity << " xe\n";
    if (vehicles == nullptr) return;
    for (int i = 0; i < currentVehicleCount; ++i) {
        if (vehicles[i] != nullptr) {
            vehicles[i]->display();
        }
    }
}

bool ParkingLot::hasVehicleType(const string& type, int& count, double& totalFee) const {
    count = 0;
    totalFee = 0.0;
    if (vehicles == nullptr) return false;

    for (int i = 0; i < currentVehicleCount; ++i) {
        if (vehicles[i] != nullptr && vehicles[i]->getVehicleType() == type) {
            ++count;
            totalFee += vehicles[i]->calculateFee();
        }
    }
    return count > 0;
}


class SystemInterface {
private:
    int lotCapacity;
    int currentLotCount;
    ParkingLot** parkingLots;

    int nhap_so_nguyen(const string& thong_bao);
    int generateRandomID();
    bool isParkIDExists(int pID) const;
    bool isVehicleIDExists(int vID) const;
    void clearLots();

public:
    SystemInterface();
    ~SystemInterface();
    SystemInterface(const SystemInterface& other);
    SystemInterface& operator=(const SystemInterface& other);

    void resizeSystem(int newCapacity);
    void runMenu();
};

void SystemInterface::clearLots() {
    if (parkingLots != nullptr) {
        for (int i = 0; i < currentLotCount; ++i) {
            delete parkingLots[i];
            parkingLots[i] = nullptr;
        }
        delete[] parkingLots;
        parkingLots = nullptr;
    }
    currentLotCount = 0;
    lotCapacity = 0;
}

SystemInterface::SystemInterface()
    : lotCapacity(5), currentLotCount(0), parkingLots(nullptr) {
    srand(static_cast<unsigned int>(time(0)));
    parkingLots = new ParkingLot*[lotCapacity];
    for (int i = 0; i < lotCapacity; ++i) {
        parkingLots[i] = nullptr;
    }
}

SystemInterface::~SystemInterface() {
    clearLots();
}

SystemInterface::SystemInterface(const SystemInterface& other)
    : lotCapacity(other.lotCapacity),
      currentLotCount(other.currentLotCount),
      parkingLots(nullptr) {
    parkingLots = new ParkingLot*[lotCapacity];
    for (int i = 0; i < lotCapacity; ++i) {
        parkingLots[i] = nullptr;
    }
    for (int i = 0; i < currentLotCount; ++i) {
        if (other.parkingLots[i] != nullptr) {
            parkingLots[i] = new ParkingLot(*other.parkingLots[i]);
        }
    }
}

SystemInterface& SystemInterface::operator=(const SystemInterface& other) {
    if (this == &other) return *this;

    clearLots();

    lotCapacity = other.lotCapacity;
    currentLotCount = other.currentLotCount;

    parkingLots = new ParkingLot*[lotCapacity];
    for (int i = 0; i < lotCapacity; ++i) {
        parkingLots[i] = nullptr;
    }
    for (int i = 0; i < currentLotCount; ++i) {
        if (other.parkingLots[i] != nullptr) {
            parkingLots[i] = new ParkingLot(*other.parkingLots[i]);
        }
    }

    return *this;
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
    if (parkingLots == nullptr) return false;
    for (int i = 0; i < currentLotCount; ++i) {
        if (parkingLots[i] != nullptr && parkingLots[i]->getParkID() == pID) {
            return true;
        }
    }
    return false;
}

bool SystemInterface::isVehicleIDExists(int vID) const {
    if (parkingLots == nullptr) return false;
    for (int i = 0; i < currentLotCount; ++i) {
        if (parkingLots[i] != nullptr && parkingLots[i]->containsVehicle(vID)) {
            return true;
        }
    }
    return false;
}

void SystemInterface::resizeSystem(int newCapacity) {
    if (newCapacity <= 0) newCapacity = 1;
    if (newCapacity < currentLotCount) newCapacity = currentLotCount;

    ParkingLot** newArr = new ParkingLot*[newCapacity];
    for (int i = 0; i < newCapacity; ++i) {
        newArr[i] = nullptr;
    }

    for (int i = 0; i < currentLotCount; ++i) {
        newArr[i] = parkingLots[i];
        if (parkingLots != nullptr) {
            parkingLots[i] = nullptr;
        }
    }

    delete[] parkingLots;
    parkingLots = newArr;
    lotCapacity = newCapacity;
}

void SystemInterface::runMenu() {
    int chon;
    while (true) {
        cout << "\n===== QUAN LY BAI DO XE (GIAI DOAN 3) =====\n";
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
                if (currentLotCount >= lotCapacity) {
                    int nextCap = (lotCapacity > 0) ? lotCapacity * 2 : 5;
                    resizeSystem(nextCap);
                    cout << "Thong bao: He thong tu dong mo rong suc chua bai do len " << lotCapacity << "\n";
                }

                int ma = nhap_so_nguyen("Ma bai: ");
                char kt;
                cout << "Kich thuoc (S/M/L): ";
                cin >> kt;
                kt = static_cast<char>(toupper(static_cast<unsigned char>(kt)));
                while (kt != 'S' && kt != 'M' && kt != 'L') {
                    cout << "Kich thuoc (S/M/L): ";
                    cin >> kt;
                    kt = static_cast<char>(toupper(static_cast<unsigned char>(kt)));
                }

                if (isParkIDExists(ma)) {
                    int id_moi = generateRandomID();
                    while (isParkIDExists(id_moi)) id_moi = generateRandomID();
                    cout << "Canh bao: Ma bai " << ma << " da ton tai! Tu dong gan ID: " << id_moi << "\n";
                    ma = id_moi;
                }

                parkingLots[currentLotCount] = new ParkingLot(ma, kt);
                currentLotCount++;
                cout << "Da tao bai " << ma << "\n";
                break;
            }

            case 2: {
                if (currentLotCount == 0 || parkingLots == nullptr) {
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

                Vehicle v(ma, loai, gio, tg);
                ParkingLot* targetLot = nullptr;
                for (int i = 0; i < currentLotCount; ++i) {
                    if (parkingLots[i] != nullptr &&
                        parkingLots[i]->getCurrentVehicleCount() < parkingLots[i]->getCapacity()) {
                        targetLot = parkingLots[i];
                        break;
                    }
                }

                if (targetLot == nullptr && parkingLots[0] != nullptr) {
                    targetLot = parkingLots[0];
                }

                if (targetLot != nullptr && targetLot->addVehicle(v)) {
                    cout << "Da them xe " << ma << " vao bai " << targetLot->getParkID()
                         << " | Phi: $" << fixed << setprecision(2) << v.calculateFee() << "\n";
                } else {
                    cout << "Khong the them xe!\n";
                }
                break;
            }

            case 3: {
                int ma = nhap_so_nguyen("Ma bai can xoa: ");
                int idx = -1;
                for (int i = 0; i < currentLotCount; ++i) {
                    if (parkingLots[i] != nullptr && parkingLots[i]->getParkID() == ma) {
                        idx = i;
                        break;
                    }
                }

                if (idx != -1) {
                    delete parkingLots[idx];
                    parkingLots[idx] = nullptr;

                    for (int i = idx; i < currentLotCount - 1; ++i) {
                        parkingLots[i] = parkingLots[i + 1];
                    }

                    parkingLots[currentLotCount - 1] = nullptr;
                    currentLotCount--;
                    cout << "Da xoa bai " << ma << "\n";
                } else {
                    cout << "Khong tim thay bai " << ma << "\n";
                }
                break;
            }

            case 4: {
                int ma = nhap_so_nguyen("Ma xe can lay ra: ");
                bool ok = false;
                for (int i = 0; i < currentLotCount; ++i) {
                    if (parkingLots[i] != nullptr && parkingLots[i]->removeVehicle(ma)) {
                        ok = true;
                        break;
                    }
                }
                cout << (ok ? "Da lay xe ra\n" : "Khong tim thay xe\n");
                break;
            }

            case 5: {
                if (parkingLots == nullptr || currentLotCount == 0) {
                    cout << "Chua co bai do nao!\n";
                    break;
                }
                for (int i = 0; i < currentLotCount; ++i) {
                    if (parkingLots[i] != nullptr) {
                        parkingLots[i]->display();
                    }
                }
                break;
            }

            case 6: {
                int ma = nhap_so_nguyen("Ma bai: ");
                bool found = false;
                for (int i = 0; i < currentLotCount; ++i) {
                    if (parkingLots[i] != nullptr && parkingLots[i]->getParkID() == ma) {
                        parkingLots[i]->display();
                        found = true;
                        break;
                    }
                }
                if (!found) {
                    cout << "Khong tim thay bai " << ma << "\n";
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
                    if (parkingLots[i] != nullptr && parkingLots[i]->hasVehicleType(loai, dem, tong)) {
                        parkingLots[i]->display();
                    }
                }
                break;
            }

            case 8: {
                string loai_arr[3] = {"Car", "Motorcycle", "Bicycle"};
                for (int t = 0; t < 3; ++t) {
                    int dem_tong = 0;
                    double tong_phi_all = 0.0;
                    int d;
                    double s;

                    for (int i = 0; i < currentLotCount; ++i) {
                        if (parkingLots[i] != nullptr && parkingLots[i]->hasVehicleType(loai_arr[t], d, s)) {
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
                clearLots();
                cout << "Da giai phong toan bo bo nho. Tam biet!\n";
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
