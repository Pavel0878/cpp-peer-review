#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <iomanip>

using namespace std;
using namespace string_literals;

static const int USER_COUNT = 100'000;
static const int PAGE_COUNT = 1'000;

void StimulatorBooks(istream& in_stream) {
	vector<int> user_page(USER_COUNT, 0);
	vector<int> page_quantity_user(PAGE_COUNT, 0);

	int request_count;
	in_stream >> request_count;
	for (int i = 0; i < request_count; ++i) {
		string request;
		int id = 0;
		int page = 0;
		in_stream >> request >> id;
		if (request == "READ"s) {
			in_stream >> page;
            int m = user_page[id] == 0 ? 0 : user_page[id] + 1;
			for (; m <= page; m++) {
				page_quantity_user[m] ++;
			}
			user_page[id] = page;
		}
		if (request == "CHEER") {
			if (user_page[id] == 0) {
				cout << 0 << endl;
			}
			else if (page_quantity_user[1] == 1) {
				cout << 1 << endl;
			}else{
				double result = ((page_quantity_user[1]-1)-((page_quantity_user[user_page[id]])-1))*1.0/(page_quantity_user[1]-1);
				cout << setprecision(6) << result << endl;
			}
		}
	}
}

int main() {
	StimulatorBooks(cin);
}