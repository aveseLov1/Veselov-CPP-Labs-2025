#include <iostream>

int main(int, char**) {
    int hours = 0;
    int minutes = 0;

    constexpr int min_hours = 0;
    constexpr int max_hours = 23;
    constexpr int min_minutes = 0;
    constexpr int max_minutes = 59;
    constexpr int noon_hours = 12;
    constexpr int midnight_hours = 0;
    constexpr int display_hours_threshold = 12;
    constexpr int singular_hour_1 = 1;
    constexpr int singular_hour_21 = 21;
    constexpr int plural_hours_min = 2;
    constexpr int plural_hours_max = 4;
    constexpr int plural_hours_min_2 = 22;
    constexpr int plural_hours_max_2 = 23;
    constexpr int modulo_base = 10;
    constexpr int minutes_exception_11 = 11;
    constexpr int minutes_small_threshold = 10;
    constexpr int minutes_large_threshold = 20;
    constexpr int morning_start = 5;
    constexpr int afternoon_start = 12;
    constexpr int evening_start = 18;

    std::cout << "Введите время (часы и минуты через пробел): ";
    std::cin >> hours >> minutes;

    if (hours < min_hours || hours > max_hours || minutes < min_minutes || minutes > max_minutes) {
        std::cout << "Введены недопустимые данные!" << std::endl;
        return 1;
    }

    if (hours == midnight_hours && minutes == min_minutes) {
        std::cout << "полночь" << std::endl;
        return 0;
    }
    if (hours == noon_hours && minutes == min_minutes) {
        std::cout << "полдень" << std::endl;
        return 0;
    }

    int display_hours = hours > display_hours_threshold ? hours - display_hours_threshold : hours;

    std::cout << display_hours << " ";

    (display_hours == singular_hour_1 || display_hours == singular_hour_21) ? std::cout << "час"
    : ((display_hours >= plural_hours_min && display_hours <= plural_hours_max) ||
       (display_hours >= plural_hours_min_2 && display_hours <= plural_hours_max_2))
        ? std::cout << "часа"
        : std::cout << "часов";

    if (minutes != min_minutes) {
        std::cout << " " << minutes << " ";
        (minutes % modulo_base == singular_hour_1 && minutes != minutes_exception_11) ? std::cout << "минута"
        : ((minutes % modulo_base >= plural_hours_min && minutes % modulo_base <= plural_hours_max) &&
           (minutes < minutes_small_threshold || minutes > minutes_large_threshold))
            ? std::cout << "минуты"
            : std::cout << "минут";
        std::cout << " ";
        (hours >= morning_start && hours < afternoon_start) ? std::cout << "утра"
        : (hours >= afternoon_start && hours < evening_start) ? std::cout << "дня"
        : (hours >= evening_start) ? std::cout << "вечера"
                        : std::cout << "ночи";
    } else {
        std::cout << " ";
        (hours >= morning_start && hours < afternoon_start) ? std::cout << "утра"
        : (hours >= afternoon_start && hours < evening_start) ? std::cout << "дня"
        : (hours >= evening_start) ? std::cout << "вечера"
                        : std::cout << "ночи";
        std::cout << " ровно";
    }

    std::cout << std::endl;

    return 0;
}


// #include <iostream>

// int main(int, char**) {
//     int hours, minutes;

//     std::cout << "Введите время (часы и минуты через пробел): ";
//     std::cin >> hours >> minutes;

//     if (hours < 0 || hours > 23 || minutes < 0 || minutes > 59) {
//         std::cout << "Введены недопустимые данные!" << std::endl;
//         return 1;
//     }

//     if (hours == 0 && minutes == 0) {
//         std::cout << "полночь" << std::endl;
//         return 0;
//     }
//     if (hours == 12 && minutes == 0) {
//         std::cout << "полдень" << std::endl;
//         return 0;
//     }

//     int display_hours = hours > 12 ? hours - 12 : hours;

//     std::cout << display_hours << " ";

//     (display_hours == 1 || display_hours == 21)                                                    ? std::cout << "час"
//     : ((display_hours >= 2 && display_hours <= 4) || (display_hours >= 22 && display_hours <= 23)) ? std::cout << "часа"
//                                                                                                    : std::cout << "часов";

//     if (minutes != 0) {
//         std::cout << " " << minutes << " ";
//         (minutes % 10 == 1 && minutes != 11)                                           ? std::cout << "минута"
//         : ((minutes % 10 >= 2 && minutes % 10 <= 4) && (minutes < 10 || minutes > 20)) ? std::cout << "минуты"
//                                                                                        : std::cout << "минут";
//         std::cout << " ";
//         (hours >= 5 && hours < 12)    ? std::cout << "утра"
//         : (hours >= 12 && hours < 18) ? std::cout << "дня"
//         : (hours >= 18)               ? std::cout << "вечера"
//                                       : std::cout << "ночи";
//     } else {
//         std::cout << " ";
//         (hours >= 5 && hours < 12)    ? std::cout << "утра"
//         : (hours >= 12 && hours < 18) ? std::cout << "дня"
//         : (hours >= 18)               ? std::cout << "вечера"
//                                       : std::cout << "ночи";
//         std::cout << " ровно";
//     }

//     std::cout << std::endl;

//     return 0;
// }

// #include <iostream>

// int main(int, char**) {
//     int hours, minutes;

//     cout << "Введите время (часы и минуты через пробел): ";
//     cin >> hours >> minutes;

//     if (hours < 0 || hours > 23 || minutes < 0 || minutes > 59) {
//         cout << "Введены недопустимые данные!" << endl;
//         return 1;
//     }

//     if (hours == 0 && minutes == 0) {
//         cout << "полночь" << endl;
//         return 0;
//     }
//     if (hours == 12 && minutes == 0) {
//         cout << "полдень" << endl;
//         return 0;
//     }

//     int display_hours = hours > 12 ? hours - 12 : hours;

//     cout << display_hours << " ";

//     (display_hours == 1 || display_hours == 21) ? cout << "час" :
//     ((display_hours >= 2 && display_hours <= 4) || (display_hours >= 22 && display_hours <= 23)) ?
//     cout << "часа" : cout << "часов";

//     if (minutes != 0) {
//         cout << " " << minutes << " ";
//         (minutes % 10 == 1 && minutes != 11) ? cout << "минута" :
//         ((minutes % 10 >= 2 && minutes % 10 <= 4) && (minutes < 10 || minutes > 20)) ?
//         cout << "минуты" : cout << "минут";
//         cout << " ";
//         (hours >= 5 && hours < 12) ? cout << "утра" :
//         (hours >= 12 && hours < 18) ? cout << "дня" :
//         (hours >= 18) ? cout << "вечера" : cout << "ночи";
//     } else {
//         cout << " ";
//         (hours >= 5 && hours < 12) ? cout << "утра" :
//         (hours >= 12 && hours < 18) ? cout << "дня" :
//         (hours >= 18) ? cout << "вечера" : cout << "ночи";
//         cout << " ровно";
//     }

//     // cout << " ";
//     // (hours >= 5 && hours < 12) ? cout << "утра" :
//     // (hours >= 12 && hours < 18) ? cout << "дня" :
//     // (hours >= 18) ? cout << "вечера" : cout << "ночи";

//     cout << endl;

//     return 0;
// }

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
