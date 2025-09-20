#include <iostream>

using namespace std;

int main(int, char**) {
    int hours, minutes;

    cout << "Введите время (часы и минуты через пробел): ";
    cin >> hours >> minutes;

    if (hours < 0 || hours > 23 || minutes < 0 || minutes > 59) {
        cout << "Введены недопустимые данные!" << endl;
        return 1;
    }

    if (hours == 0 && minutes == 0) {
        cout << "полночь" << endl;
        return 0;
    }
    if (hours == 12 && minutes == 0) {
        cout << "полдень" << endl;
        return 0;
    }

    int display_hours = hours > 12 ? hours - 12 : hours;

    cout << display_hours << " ";

    (display_hours == 1 || display_hours == 21) ? cout << "час" :
    ((display_hours >= 2 && display_hours <= 4) || (display_hours >= 22 && display_hours <= 23)) ?
    cout << "часа" : cout << "часов";

    if (minutes != 0) {
        cout << " " << minutes << " ";
        (minutes % 10 == 1 && minutes != 11) ? cout << "минута" :
        ((minutes % 10 >= 2 && minutes % 10 <= 4) && (minutes < 10 || minutes > 20)) ?
        cout << "минуты" : cout << "минут";
        cout << " ";
        (hours >= 5 && hours < 12) ? cout << "утра" :
        (hours >= 12 && hours < 18) ? cout << "дня" :
        (hours >= 18) ? cout << "вечера" : cout << "ночи";
    } else {
        cout << " ";
        (hours >= 5 && hours < 12) ? cout << "утра" :
        (hours >= 12 && hours < 18) ? cout << "дня" :
        (hours >= 18) ? cout << "вечера" : cout << "ночи";
        cout << " ровно";
    }

    // cout << " ";
    // (hours >= 5 && hours < 12) ? cout << "утра" :
    // (hours >= 12 && hours < 18) ? cout << "дня" :
    // (hours >= 18) ? cout << "вечера" : cout << "ночи";

    cout << endl;

    return 0;
}




// #include <iostream>

// using namespace std;

// int main() {
//     int hours, minutes;

//     cout << "Введите время (часы и минуты через пробел): ";
//     cin >> hours >> minutes;

//     bool invalid = (hours < 0) || (hours > 23) || (minutes < 0) || (minutes > 59);
//     invalid && (cout << "Введены недопустимые данные!" << endl, exit(1), 0);

//     bool is_midnight = (hours == 0) && (minutes == 0);
//     bool is_noon = (hours == 12) && (minutes == 0);

//     is_midnight && (cout << "полночь" << endl, exit(0), 0);
//     is_noon && (cout << "полдень" << endl, exit(0), 0);

//     int display_hours = hours > 12 ? hours - 12 : hours;

//     cout << display_hours << " ";

//     (display_hours == 1 || display_hours == 21) ? cout << "час" :
//     ((display_hours >= 2 && display_hours <= 4) || (display_hours >= 22 && display_hours <= 23)) ?
//     cout << "часа" : cout << "часов";

//     minutes == 0 ? cout << " ровно" : (
//         cout << " " << minutes << " ",
//         (minutes % 10 == 1 && minutes != 11) ? cout << "минута" :
//         ((minutes % 10 >= 2 && minutes % 10 <= 4) && (minutes < 10 || minutes > 20)) ?
//         cout << "минуты" : cout << "минут"
//     );

//     cout << " ";
//     (hours >= 5 && hours < 12) ? cout << "утра" :
//     (hours >= 12 && hours < 18) ? cout << "дня" :
//     (hours >= 18) ? cout << "вечера" : cout << "ночи";

//     cout << endl;

//     return 0;
// }
