#include <iostream>
#include <exception>
#include <string>
#include <map>
#include <set>
#include <iomanip>
#include <sstream>
using namespace std;
class Date {
public:
    Date(int y, int m, int d) {
        year = y;
        month = m;
        day = d;
    }

    int GetYear() const {
        return year;
    }
    int GetMonth() const {
        return month;
    }
    int GetDay() const {
        return day;
    }

private:
    int year;
    int month;
    int day;
};
bool operator<(const Date& lhs, const Date& rhs) {
    if (lhs.GetYear() == rhs.GetYear()) {
        if (lhs.GetMonth() == rhs.GetMonth()) {
            return lhs.GetDay() < rhs.GetDay();
        }
        return lhs.GetMonth() < rhs.GetMonth();
    }
    return lhs.GetYear() < rhs.GetYear();
}
class Database {
public:
    void AddEvent(const Date& date, const string& event) {
        events[date].insert(event);
    }
    bool DeleteEvent(const Date& date, const string& event) {
        if (events.count(date) && events[date].count(event)) {
            events[date].erase(event);
            if (events[date].empty()) {
                events.erase(date);
            }
            return true;
        }
        return false;
    }
    int  DeleteDate(const Date& date) {
        if (events.count(date)) {
            int s = events[date].size();
            events.erase(date);
            return s;
        }
        return 0;
    }
    void Find(const Date& date) const {
        if (events.count(date)) {
            set<string> temp = events.at(date);
            bool isFirst = true;
            for (auto item : temp) {
                cout << item << endl;
            }
        }
    }

    void Print() const {
        for (auto key : events) {
            for (auto a : key.second) {
                cout << setw(4) << setfill('0') << key.first.GetYear();
                cout << "-";
                cout << setw(2) << setfill('0') << key.first.GetMonth();
                cout << "-";
                cout << setw(2) << setfill('0') << key.first.GetDay();
                cout << " ";
                cout << a;
                cout << endl;
            }
        }
    }

private:
    map<Date, set<string>> events;
};
void parsing(const string& data, int& year, int& month, int& day) {
    stringstream stream(data);
    char del1, del2;

    stream >> year >> del1 >> month >> del2 >> day;

    if (!stream || stream.peek() != EOF || del1 != '-' || del2 != '-') {
        throw invalid_argument("Wrong date format: " + data);
    }
    if (month < 1 || month > 12) {
        throw invalid_argument("Month value is invalid: " + to_string(month));
    }

    if (day < 1 || day > 31) {
        throw invalid_argument("Day value is invalid: " + to_string(day));
    }
}
int main() {
    Database db;
    
    set<string> commands = { "Add", "Del", "Find", "Print", "Stop" };

    cout << "Welcome!" << endl;

    string command;
    while (getline(cin, command)) {
        stringstream stream(command);
        string com;
        stream >> com;
        if (com.empty()) {
            continue;
        }
        if (!commands.count(com)) {
            cout << "Unknown command: " << com;
            return 0;
        }
        if (com == "Print") {
            db.Print();
        }
        else if (com == "Stop") {
            cout << "The pogramm hes ended!";
            return 0;
        }
        else if (com == "Add") {
            string data, event;
            stream >> data >> event;
            int year, month, day;
            try {
                parsing(data, year, month, day);
            }
            catch (const invalid_argument& e) {
                cout << e.what();
                return 0;
            }
            Date date(year, month, day);
            db.AddEvent(date, event);
        }
        else if (com == "Find") {
            string data;
            stream >> data;
            int year, month, day;
            try {
                parsing(data, year, month, day);
            }
            catch (const invalid_argument& e) {
                cout << e.what();
                return 0;
            }
            Date date(year, month, day);
            db.Find(date);
        }
        else {
            string data;
            stream >> data;
            int year, month, day;
            try {
                parsing(data, year, month, day);
            }
            catch (const invalid_argument& e) {
                cout << e.what();
                return 0;
            }
            Date date(year, month, day);
            if (stream && stream.eof()) {
                int res = db.DeleteDate(date);
                cout << "Deleted " << res << " events" << endl;
            }
            else {
                string event;
                stream >> event;
                if (db.DeleteEvent(date, event)) {
                    cout << "Deleted successfully" << endl;
                }
                else {
                    cout << "Event not found" << endl;
                }
            }
        }
    }
    return 0;
}
