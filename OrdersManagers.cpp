// 23 - 12 - 23 

#include<bits/stdc++.h>

using namespace std;

#define read() ios_base::sync_with_stdio(0);cin.tie(0);cout.tie(0)
#define day() time_t now = time(0);char* x = ctime(&now);cerr<<"Right Now Is : "<<x<<"\n"

#define ii pair<int,int>
#define X first
#define Y second 

const long long MAX = (int)1e5 + 5;
const long long INF = (int)1e9;
const long long MOD = (int)1e9 + 7;

struct Dish {
    string maMon;
    string thoiGian;
    int soLuongDat;
    int soLuongTra;
    string thoiGianCapNhat;
    string ghiChu;
    Dish *next = NULL;
    
    Dish(){
    	maMon = "system";
    	thoiGian = "system";
    	thoiGianCapNhat = "system";
    	ghiChu = "system";
    	soLuongDat = 0;
    	soLuongTra = 0;
    	next = NULL;
    }
};

enum TrangThai {
    DANG_PHUC_VU, // 0 
    DA_THANH_TOAN, // 1 
    DON_HUY // 2 
};

struct Order {
    string thoiGian;
    string tenNhanVien; 
    int maBan;
    Dish* danhSachMon;
    int tongSoMon = 0;
    int tongSoDiaDat = 0;
    int tongSoMonTra = 0;
    int tongSoDiaTra = 0;
    string thoiGianCapNhat;
    int trangThai; // 0 1 2 
    Order *next = NULL;
    
    Order(int maban = 0,string tennhanvien = "System",string thoigian = "System"){
    	thoiGian = thoigian;
    	tenNhanVien = tennhanvien;
    	maBan = maban;
    	danhSachMon = new Dish();
    	tongSoMon = 0;
	    tongSoDiaDat = 0;
	    tongSoMonTra = 0;
	    tongSoDiaTra = 0;
	   	thoiGianCapNhat = thoigian;
	    trangThai = 0;
	    Order *next = NULL;
    }
    void print(){
    	Dish *rt = danhSachMon;
    	int cnt = 0;
    	while(rt != NULL){
    		cnt++;
    		if(cnt > 1)
    		cout<< rt -> maMon << ": " << rt -> soLuongDat << "\n";
    		rt = rt -> next;
    	}
    }
    void clear(){
    	thoiGian = "000";
    	tenNhanVien = "System";
    	maBan = -1;
    	danhSachMon = new Dish();
    	tongSoMon = 0;
	    tongSoDiaDat = 0;
	    tongSoMonTra = 0;
	    tongSoDiaTra = 0;
	   	thoiGianCapNhat = "000";
	    trangThai = 0;
    }
};
Order *root;

Order* search_order(int table_id) {
	Order *rt = root;
	if(rt -> next == NULL){
		rt -> next = new Order();
		return rt -> next;
	}
	while(rt -> next != NULL && rt -> maBan != table_id){
		rt = rt -> next;
	}
	if(rt -> maBan != table_id || rt -> trangThai > 0){
		rt -> next = new Order();
		return rt -> next;
	}
	return rt;
}
void Print_Order(int table_id){
	Order *rt = search_order(table_id);
	if(rt -> maBan != table_id){
		cout << "Chua co order\n";
		return;
	}
	cout << rt -> thoiGian << "\n";
	cout << "Ban " << rt -> maBan << '\n';
	rt -> print();
	cout << "Tong so mon: " << rt -> tongSoMon << '\n';
	cout << "Tong so dia: " << rt -> tongSoDiaDat << '\n';
	cout << "\n";
}

Order* create_order(int table_id, string tenNV, string tgian) {
    Order *rt = search_order(table_id);
    if(rt -> trangThai > 0){
    	rt -> clear();
    }
    rt -> maBan = table_id;
    rt -> thoiGian = tgian;
    rt -> tenNhanVien = tenNV;
    return rt;
}

string currentTime() {
    time_t now = time(0);
    tm* ltm = localtime(&now);
    char buf[20];
    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", ltm);
    return string(buf);
}
Dish* Search_dish(Dish* rootx,string MaMon){
	Dish* rt = rootx;
	while(rt -> next != NULL && rt -> maMon != MaMon)rt = rt -> next;
	if(rt -> maMon != MaMon){
		rt -> next = new Dish();
		return rt -> next;
	}
	return rt;
}
bool add_dish(int table_id, string maMon, int quantity, string notes) {
    Order* order = search_order(table_id);
    if (order == NULL) {
        order = create_order(table_id, "System", currentTime());
        if (order == NULL) return 0;
    }
    order -> maBan = table_id;
	if(order -> trangThai > 0)order -> clear();
	
    // Tạo món mới
    Dish *newDish = Search_dish(order -> danhSachMon,maMon);
    //cout << maMon << "---\n";
   	//cout << table_id << " &&& " << maMon << " &&& " << quantity << " &&" << notes << "\n";
    newDish -> maMon = maMon;
    newDish->thoiGian = currentTime();
    newDish->thoiGianCapNhat = currentTime();
    newDish->soLuongDat = quantity;
    newDish->soLuongTra = 0;
    newDish->ghiChu = notes;
    order -> tongSoMon++;
    order -> tongSoDiaDat += quantity;

    return 1;
}
// 
bool update_dish(int table_id, string maMon, int quantity) {
    Order* order = search_order(table_id);
    //cout << table_id << '\n';
    if (order == NULL) return 0;

    Dish* dish = order->danhSachMon;
    while (dish != NULL) {
    	//cout << maMon << " + " << dish -> maMon << '\n';
        if (dish->maMon == maMon) {
            if (quantity > dish->soLuongDat) return 0;
            dish->soLuongTra = quantity;
            dish->thoiGianCapNhat = currentTime();
			
            order->tongSoMonTra++;
            order->tongSoDiaTra += quantity;
            order->thoiGianCapNhat = currentTime();
            return 1;
        }
        dish = dish->next;
    }
    return 0;
}
bool cancel_dish(int table_id, string maMon, string notes) {
    Order* order = search_order(table_id);
    if (order == NULL) return 0;

    Dish* dish = order->danhSachMon;
    while (dish != NULL) {
    	//cout << dish -> maMon << " " << maMon << "\n";
        if (dish->maMon == maMon && dish -> soLuongTra == 0) {
            dish->ghiChu = notes;
            dish->thoiGianCapNhat = currentTime();
            return 1;
        }
        dish = dish->next;
    }
    return 0;
}
// 
bool cancel_order(int table_id) {
    Order* curr = root;
    while (curr != NULL) {
        if (curr -> maBan == table_id && curr->trangThai == DANG_PHUC_VU) {
            if (curr -> tongSoDiaTra == 0) {
                curr -> trangThai = DON_HUY;
                return 1;
            } else {
                return 0;
            }
        }
        curr = curr->next;
    }
    return 0;
}

void create_bill(int table_id) {
    Order* current = root;
    int found = 0;

    while (current != NULL) {
        if (current->maBan == table_id && current->trangThai == DANG_PHUC_VU) {
            cout << "=== Hoa don ban "<< current->maBan << "===\n";
            cout << "Thoi gian: " << current->thoiGian << "\n";
            Dish* d = current->danhSachMon;
            while (d != NULL) {
            	cout << "Mon: " << d -> maMon << " - SL tra: " << d -> soLuongTra << " - Ghi chu: " << d -> ghiChu << "\n";
               // printf("Mon: %s - SL tra: %d - Ghi chu: %s\n", d->maMon, d->soLuongTra, d->ghiChu);
                d = d->next;
            }
            current->trangThai = DA_THANH_TOAN;
            found = 1;
        }
        current = current->next;
    }
// fclose(file);
    if (!found) printf("Khong co don hang de thanh toan\n");
}

char k;
string s;
signed main(){
	
	read();
	
//	if(ifstream("orders_manager.inp")){
//		freopen("orders_manager.inp",'r',stdout);
//	}
	root = new Order();
	while(cin >> k >> s){
		if(s == "print_order"){
			int table_id;
			cin >> table_id;
			Print_Order(table_id);
		}else if(s == "create_order"){
			string tenNV;
			int table_id;
			string thoigian;
			
			while(cin >> tenNV){
				if(tenNV == "#")break;
				cin >> table_id;
				getline(cin,thoigian);
				create_order(table_id,tenNV,thoigian);
			}
		}else if(s == "add_dish"){
			string tenNV;
			int maban,slg;
			string tenMon,ghichu;
			while(cin >> tenNV){
				if(tenNV == "#")break;
				cin >> maban >> tenMon >> slg >> ghichu;
				bool c = add_dish(maban,tenMon,slg,ghichu);
				if(c)cout << "them mon thanh cong\n";
				else cout << "them mon that bai\n";
			}
		}else if(s == "update_dish"){
			string tenNV;
			int maban,slg;
			string tenMon;
			while(cin >> tenNV){
				if(tenNV == "#")break;
				cin >> maban >> tenMon >> slg;
				bool c = update_dish(maban,tenMon,slg);
				if(c)cout << "tra mon thanh cong\n";
				else cout << "tra mon that bai\n";
			}
		}else if(s == "cancel_dish"){
			string tenNV;
			int maban;
			string tenMon,ghichu;
			while(cin >> tenNV){
				if(tenNV == "#")break;
				cin >> maban >> tenMon >> ghichu;
				bool c = cancel_dish(maban, tenMon, ghichu);
				if(c)cout << "huy mon thanh cong\n";
				else cout << "huy mon that bai\n";
			}
		}else if(s == "cancel_order"){
			string tenNV;
			int maban;
			while(cin >> tenNV){
				if(tenNV == "#")break;
				cin >> maban;
				bool c = cancel_order(maban);
				if(c)cout << "huy ban thanh cong\n";
				else cout << "huy ban that bai\n";
			}
		}else if(s == "create_bill"){
			string tenNV;
			int maban;
			while(cin >> tenNV){
				if(tenNV == "#")break;
				cin >> maban;
				create_bill(maban);
				
			}
		}
	}
    return 0;
}
