#include <iostream>
#include <vector>
#include <iomanip>
#include <fstream>
#include <limits>
#include <sstream>
using namespace std;
// ================== GIAO DICH ==================
class GiaoDich {
private:
    string loai;
    double soTien;
public:
    GiaoDich(string l, double s) { 
        loai = l; soTien = s;
    }

    string getLoai() { return loai; } 
    double getSoTien() { return soTien; }

    void hienThi() { 
        cout << loai << " | So tien: " << soTien << endl;
    }
};

// ================== TAI KHOAN ==================
class TaiKhoan { 
protected:
	vector<GiaoDich> lichSu;
    string soTK;
    double soDu;
public:
    TaiKhoan(string stk, double sd) {
        soTK = stk;
        soDu = sd;
    }
    virtual ~TaiKhoan() {}

    virtual void hienThi() { 
    cout << "So TK: " << soTK
         << " | So du: " << fixed << setprecision(0) << soDu
         << " | Lai: " << tinhLai() << endl;
}

    virtual void napTien(double tien) {
    if (tien > 0) {
        soDu += tien;
        lichSu.push_back(GiaoDich("Nap", tien));
    }
}

    virtual void rutTien(double tien) {
    if (tien <= 0) {
        cout << "So tien khong hop le!\n";
    } else if (soDu >= tien) {
        soDu -= tien;
        lichSu.push_back(GiaoDich("Rut", tien));
    } else cout << "Khong du tien!\n";
}

    virtual double tinhLai() { return 0; } 

    string getSoTK() { return soTK; }
    double getSoDu() { return soDu; }
    
    void hienThiLichSu() {  
    for (auto &gd : lichSu)
        gd.hienThi();
}

};


// ===== TIET KIEM ===== kt
class TietKiem : public TaiKhoan { 
public:
    TietKiem(string stk, double sd) : TaiKhoan(stk, sd) {}
    double tinhLai() override { return soDu * 0.05; } 
};

// ===== THANH TOAN =====kt
class ThanhToan : public TaiKhoan {
public:
    ThanhToan(string stk, double sd) : TaiKhoan(stk, sd) {}
    double tinhLai() override { return soDu * 0.01; }
};

// ================== KHACH HANG ==================
class KhachHang {
private:
	
    string ten, cccd;
    vector<TaiKhoan*> dsTK;
public:
	string getCCCD() { return cccd; }
    KhachHang(string t, string c) {
        ten = t; cccd = c;
    }

    void themTaiKhoan(TaiKhoan* tk) {
        dsTK.push_back(tk);
    }

    vector<TaiKhoan*>& getDSTK() { return dsTK; }
    string getTen() { return ten; }
    
    ~KhachHang() {
    for (auto tk : dsTK) {
        delete tk;
    }
}

	
	
	void xoaTaiKhoan(string stk) {
    for (auto it = dsTK.begin(); it != dsTK.end(); ++it) {
        if ((*it)->getSoTK() == stk) { 
            delete *it;
            dsTK.erase(it);
            cout << "Da xoa tai khoan!\n";
            return;
        }
    }
    cout << "Khong tim thay tai khoan!\n";
}

};


// ================== NGAN HANG ================== 
class NganHang {
private:
    vector<KhachHang*> dsKH;

public:
    void themKhachHang() {
        string ten, cccd;
        cout << "Nhap ten: "; cin.ignore(numeric_limits<streamsize>::max(), '\n');
		 getline(cin, ten);
        cout << "Nhap CCCD: "; getline(cin, cccd);

        dsKH.push_back(new KhachHang(ten, cccd));
    }

    KhachHang* chonKhachHang() {
    for (int i = 0; i < dsKH.size(); i++) {
        cout << i << ". " << dsKH[i]->getTen()
     << " (" << dsKH[i]->getCCCD() << ")" << endl;
    }
    int chon;
    cin >> chon;

    if (chon >= 0 && chon < dsKH.size())
        return dsKH[chon];

    return nullptr;
}

    void themTaiKhoan() {
        KhachHang* kh = chonKhachHang();
        if (!kh) return;

        string stk;
        double sd;
        int loai;

        cout << "Nhap so TK: "; cin >> stk;
        cout << "Nhap so du: "; cin >> sd;
        cout << "1. Tiet kiem | 2. Thanh toan: ";
        cin >> loai;

        if (loai == 1)
            kh->themTaiKhoan(new TietKiem(stk, sd));
        else
            kh->themTaiKhoan(new ThanhToan(stk, sd));
    }

	void xoaKhachHang() {
    string cccd;
    cout << "Nhap CCCD can xoa: ";
    cin >> cccd;

    for (auto it = dsKH.begin(); it != dsKH.end(); ++it) {
        if ((*it)->getCCCD() == cccd) {
    		delete *it;
    		dsKH.erase(it);
            cout << "Da xoa!\n";
            return;
        }
    }
    cout << "Khong tim thay!\n";
}
	
	void xoaTaiKhoan() {
    string stk;
    cout << "Nhap so TK can xoa: ";
    cin >> stk;

   for (auto kh : dsKH) { 
        kh->xoaTaiKhoan(stk);
    }
}
	
    TaiKhoan* timTK(string stk) {
        for (auto &kh : dsKH)
            for (auto tk : kh->getDSTK())
                if (tk->getSoTK() == stk)
                    return tk;
        return nullptr;
    }
    
    KhachHang* timKH(string cccd) {
    for (auto &kh : dsKH)
        if (kh->getCCCD() == cccd)
            return kh;
    return nullptr;
}

    void napTien() {
        string stk; double tien;
        cout << "Nhap so TK: "; cin >> stk;
        cout << "Nhap so tien: "; cin >> tien;

        TaiKhoan* tk = timTK(stk);
        if (tk) tk->napTien(tien);
        else cout << "Khong tim thay!\n";
    }

    void rutTien() {
        string stk; double tien;
        cout << "Nhap so TK: "; cin >> stk;
        cout << "Nhap so tien: "; cin >> tien;

        TaiKhoan* tk = timTK(stk);
        if (tk) tk->rutTien(tien);
        else cout << "Khong tim thay!\n";
    }

    void chuyenTien() {
        string a, b; double t;
        cout << "TK gui: "; cin >> a;
        cout << "TK nhan: "; cin >> b;
        cout << "So tien: "; cin >> t;

        TaiKhoan* tk1 = timTK(a);
        TaiKhoan* tk2 = timTK(b);

        if (tk1 && tk2 && tk1->getSoDu() >= t) {
            tk1->rutTien(t);
            tk2->napTien(t);
            cout << "Thanh cong!\n";
        } else cout << "That bai!\n";
    }

    void hienThi() {
    for (auto kh : dsKH) {
        cout << "Khach hang: " << kh->getTen() << endl;

        if (kh->getDSTK().empty()) {
            cout << "  (Khong co tai khoan)\n";
            continue;
        }

        for (auto tk : kh->getDSTK()) {
            tk->hienThi();
            tk->hienThiLichSu();
        }
    }
}
    void thongKe() {
    int tongKH = dsKH.size();
    int tongTK = 0;
    double tongTien = 0;

    for (auto &kh : dsKH) {
        tongTK += kh->getDSTK().size();
        for (auto tk : kh->getDSTK())
            tongTien += tk->getSoDu();
    }

    cout << "Tong KH: " << tongKH << endl;
    cout << "Tong TK: " << tongTK << endl;
    cout << "Tong tien: " << tongTien << endl;
}
	
	void luuFile() {
    ofstream file("data.txt");

    for (auto &kh : dsKH) {
        file << kh->getTen() << "|" << kh->getCCCD() << endl;

        for (auto tk : kh->getDSTK()) {
            file << tk->getSoTK() << " "
                 << tk->getSoDu() << endl;
        }
        file << "#" << endl;
    }

    file.close();
    cout << "Da luu file!\n";
}

void docFile() {
    for (auto kh : dsKH) {
    delete kh;
}
	dsKH.clear(); 
    ifstream file("data.txt");

    if (!file) {
        cout << "Khong tim thay file!\n";
        return;
    }

    string line;
    KhachHang* kh = nullptr;

    while (getline(file, line)) {
        if (line == "#") {
            kh = nullptr; 
        }
        else if (line.find("|") != string::npos) {
            
            int pos = line.find("|");
            string ten = line.substr(0, pos);
            string cccd = line.substr(pos + 1);

            kh = new KhachHang(ten, cccd);
            dsKH.push_back(kh);
        }
        else {
            
            if (kh != nullptr) {
                string stk;
                double sd;

                stringstream ss(line);
                ss >> stk >> sd;

                
                kh->themTaiKhoan(new ThanhToan(stk, sd));
            }
        }
    }

    file.close();
    cout << "Da doc file!\n";
}

	~NganHang() {
    	for (auto kh : dsKH)
        	delete kh;
}

};

// ================== MAIN ==================
int main() {
    NganHang nh;
    int chon;

    do {
        cout << "\n=== MENU ===\n";
        cout << "1. Them khach hang\n";
        cout << "2. Them tai khoan\n";
        cout << "3. Nap tien\n";
        cout << "4. Rut tien\n";
        cout << "5. Chuyen tien\n";
        cout << "6. Hien thi\n";
        cout << "7. Xoa khach hang\n";
		cout << "8. Xoa tai khoan\n";
		cout << "9. Thong ke\n";
		cout << "10. Luu file\n";
		cout << "11. Doc file\n";
        cout << "0. Thoat\n";
        cout << "Chon: ";
        cin >> chon;

        switch (chon) { 
        case 1: nh.themKhachHang(); break;
        case 2: nh.themTaiKhoan(); break;
        case 3: nh.napTien(); break;
        case 4: nh.rutTien(); break;
        case 5: nh.chuyenTien(); break;
        case 6: nh.hienThi(); break;
        case 7: nh.xoaKhachHang(); break;
		case 8: nh.xoaTaiKhoan(); break;
		case 9: nh.thongKe(); break;
		case 10: nh.luuFile(); break;
		case 11: nh.docFile(); break;
        }
    } while (chon != 0);

    return 0;
}
