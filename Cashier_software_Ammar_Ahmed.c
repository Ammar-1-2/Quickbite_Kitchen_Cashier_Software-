/* Name: Ammar Ahmed
   Registration number:
   Date: 09/11/2026 */

#include <stdio.h>
#include <stdlib.h>

/*Constants (fixed values with names)
#define gives a value a name. Before the program is compiled, the computer swaps every name for its value.
 If the VAT rate changes one day, you change it in ONE place instead of hunting through the code. */

#define COMPANY_NAME      "QuickBite Kitchen" /* The name of the business. Wherever the code says BUSINESS_NAME, the text "QuickBite Kitchen" is used. */
#define CASHIER_NAME       "Ammar Ahmed" /*The cashier's name that is printed on every receipt ("Served by"). */
#define ITEM_COUNT         10           /* How many items are on the menu: 10. It is used for the size of the arrays and for every loop that goes through the menu. */
#define VAT_RATE           0.05f        /* VAT is 5%, written as the decimal 0.05 (5 divided by 100). The f at the end means "float", a number with a decimal point. */
#define DISCOUNT_RATE      0.10f        /* The discount is 10%, written as 0.10. */
#define DISCOUNT_THRESHOLD 60.00f       /* The subtotal a customer must reach to get the discount: AED 60.00 */
#define MIN_QUANTITY       1            /* The smallest quantity allowed for one entry: 1.*/
#define MAX_QUANTITY       10           /* The biggest quantity allowed for one entry: 10 */
#define NAME_SIZE          20           /* How many characters of space the customer's name gets: 20. That is 19 letters plus one hidden end-of-text marker that C always adds. */
#define PAY_CASH           1            /* A name for the number 1, so the code can say PAY_CASH instead of a "mystery" 1. */
#define PAY_CARD           2            /* A name for the number 2 (card payment). */
#define MAX_CASH           10000.00f    /* The biggest cash amount the till accepts: AED 10,000. It stops silly or broken amounts. */

/* The menu (two arrays)
The boxes are counted from 0, not 1.
These two arrays line up: box 0 of item_names goes with box 0 of item_prices, and so on.
That is how the program knows the price of each food item. The boxes are counted from 0, not 1. */
const char *item_names[ITEM_COUNT] = {                      /* item_names is an array with ITEM_COUNT (10) boxes. Each box holds a piece of text (char * means "text").
                                                            const means the list cannot be changed while the program runs. The { opens the list of values. */
    "Beef Burger", "Chicken Burger", "Cheese Pizza Slice",
    "Chicken Shawarma Wrap", "Falafel Wrap", "French Fries",
    "Chicken Nuggets", "Karak Tea", "Fresh Orange Juice", "Soft Drink"  /* The ten food names, in order. Box 0 is Beef Burger, box 1 is Chicken Burger, and so on up to box 9, Soft Drink.
                                                                        The customer sees these as numbers 1 to 10, which is the box number plus 1.*/
}; /* Ends the list of names. The semicolon is needed after the closing brace of a list. */

const float item_prices[ITEM_COUNT] = {                             /*item_prices is an array of 10 decimal numbers (float). Box 0 is the price of the item in box 0 of item_names. */
    18.00f, 16.00f, 12.00f, 14.00f, 10.00f, 8.00f, 15.00f, 3.00f, 9.00f, 4.00f  /* The ten items price's in AED, in order */
};


/*input_ended() - stop safely
if the keyboard input runs out, print a message and stop the program, so it never asks the same question forever.
*/
void input_ended(void) {                        /* Starts the function. void in front means it gives nothing back.
                                                        (void) in the brackets means nothing is passed in. The { opens its body. */

    printf("\nInput ended. Closing the till.\n");    /* Shows a message. \n means "start a new line". */

    exit(0);     /* Stops the whole program straight away. The 0 means "ended normally".
                    This is only used if the keyboard input runs out, so the program does not ask the same question forever. */
}

/* clean_line() - clear the keyboard 'waiting room'
clears the leftover typed text (like the Enter key) and reports whether anything odd was typed after the answer.
 */
int clean_line(void) {         /* Starts the function. int means it gives back a whole number: 1 (clean line) or 0 (extra junk was typed). */
    int c;                  /* A box to hold ONE typed character at a time */
    int only_spaces = 1;    /* A yes/no flag. 1 means "yes, only spaces so far". It starts as yes. */

    while ((c = getchar()) != '\n' && c != EOF) {  ;  /* A loop. It reads one character into c, then checks it.
                                                        It keeps going while the character is NOT the Enter key ('\n') AND
                                                        is not the end of the input (EOF). && means AND, != means "is not equal to". */
        if (c != ' ' && c != '\t') {        /* If the character is not a space and not a tab */
            only_spaces = 0;        /* then real text was typed, so the line is not clean. Set the flag to 0 (no). */
        }
    }
    return only_spaces;        /* Hands back the flag: 1 if the line was clean, 0 if not. */
}

/* read_integer() - read one whole number safely
In short: reads one whole number from the keyboard and reports whether it worked. Letters and decimals are rejected.
 */
int read_integer(int *number) {        /* Starts the function. It gives back 1 (worked) or 0 (did not).
                                        The * in int *number means number is the ADDRESS of a variable that belongs to whoever called the function.
                                        That lets this function fill that variable. */
    int result = scanf("%d", number);  /* scanf("%d", number) tries to read a whole number (%d means "whole number") and store it in number.
                                        scanf also reports how many values it managed to read: 1 means success, 0 means it was not a number, EOF means the input ended.
                                        We keep that report in result. */

    if (result == EOF) {
        input_ended();     /* If the input ended, call input_ended() to stop safely. Note == compares two things.
                                A single = would store a value instead.  */
    }
    if (result != 1) {
        clean_line();
        return 0;          /* If scanf did not read a number (for example the user has typed "abc"):
                                clean the line so the letters do not stay stuck, then return 0 which means "that did not work". */
    }
    return clean_line();  /* A number was read. clean_line() clears the rest of the line and returns 1 if nothing odd was typed after it,
                            or 0 if there was junk (like the "abc" in "5abc" or the ".5" in "3.5"). That answer is passed straight back.  */
}

/* ask_number_in_range() - the 'safe number question'
keeps asking until the user types a whole number inside the allowed range. This is the input checking for requirement, used for the menu choice, the quantity and the payment option.
 */
int ask_number_in_range(const char *prompt, int low, int high, const char *what) { /* Starts the function. It gives the question to show (prompt),
                                                                                    the lowest and highest allowed numbers (low, high), and a word (what) used in the error message.
                                                                                    It gives back the valid number.*/
    int number;     /* A box for the number the user types. */

    while (1) {      /* A loop that repeats forever, because 1 is always true. The only way out is the return further down.*/
        printf("%s", prompt);   /* Shows the question. %s means "put a piece of text here", and the text is prompt. */
        if (!read_integer(&number)) {   /* Tries to read a number. & means "address of", so the function can fill number. ! means NOT, so this says:
                                        if reading a clean number FAILED  */
            printf("  Invalid %s! Please type a whole number.\n", what);   /* tell the user it was not a whole number.
                                                                            The %s is replaced by the word in what, for example "menu number". */
        } else if (number < low || number > high) {      /* Otherwise, if the number is too small OR too big. || means OR.   */
            printf("  Invalid %s! It must be between %d and %d.\n", what, low, high);   /* Tell the user the allowed range. The %d signs are replaced by low and high. */
        } else {    /* Otherwise the number is fine */
            return number;    /* so leave the function and hand the number back. This is the only exit from the loop. */
        }
    }
}

/* ask_two_options() - ask a y/n or d/t question
In short: keeps asking a two-answer question (y/n or d/t) until a valid letter is typed. */

char ask_two_options(const char *question, char first, char second) {  /* Starts the function. It gets the question and the two letters that are allowed (first and second),
                                                                        for example 'y' and 'n'. It gives back one character (char).  */
    char answer;                     /* A box for ONE typed letter.  */
    int  result;                    /* A box to remember whether scanf worked. */

    while (1) {                     /* Repeat until we return a valid answer. */
        printf("%s", question);                     /* Shows the question.  */
        result = scanf(" %c", &answer);             /* Reads one character (%c means "one character").
                                                    The space before %c tells scanf to skip leftover spaces and Enter keys first.  */
        if (result == EOF) {                        /* If the input ended, stop safely. */
            input_ended();
        }
        if (clean_line()) {                         /* Only carry on if NOTHING else was typed on the line. So "yes" is rejected and "y" is accepted.  */
            if (answer >= 'A' && answer <= 'Z') {   /* Is the letter a capital (between 'A' and 'Z')?*/
                answer = (char)(answer + ('a' - 'A'));   /* Letters are stored as numbers inside the computer. Adding the gap between 'a' and 'A' turns a capital into a small letter, so 'Y' becomes 'y'.
                                                        (char) turns the number back into a letter.*/
            }
            if (answer == first || answer == second) {  /* Is the letter one of the two allowed answers? */
                return answer;         /* Yes, so hand it back and leave the function. */
            }
        }
        printf("  Please type %c or %c.\n", first, second);   /* We only get here if the answer was not valid, so tell the user which letters are allowed. The loop then asks again. */
    }
}

/* The money functions
the money maths. Four small modules: round to 2 decimals, work out the subtotal, give the 10% discount (AED 60 or more), and work out the 5% VAT
 */
float round_money(float amount) {    /* round_money takes a decimal number and gives back a decimal number (float). */
    return (float)((int)(amount * 100.0f + 0.5f)) / 100.0f; /* Rounds to 2 decimal places. Step by step: multiply by 100 (3.6 becomes 360), add 0.5,
                                                            then (int) chops off everything after the decimal point (so adding 0.5 first makes it round properly), then divide by 100 to move the point back.
                                                            This stops tiny computer errors so the numbers on the receipt add up exactly.*/
}

float calc_subtotal(int quantities[]) {  /* Works out the subtotal (requirement R3). It gives the quantities array: how many of each menu item are in the order. */
    int   i;   /* A counter for the loop. */
    float subtotal = 0.0f;   /* The running total, starting at zero.*/

    for (i = 0; i < ITEM_COUNT; i++) {   /* for loop. It sets i to 0, keeps going while i is less than 10, and adds 1 to i after every lap (i++). So i goes 0, 1, 2 ... 9, visiting every menu item.*/
        subtotal += item_prices[i] * quantities[i];   // Price times quantity for item i, added onto the running total. += means "add to what is already there".
    }
    return round_money(subtotal);                   // Hands back the subtotal, rounded to 2 decimals.
}

float calc_discount(float subtotal) {                   // Works out the discount. It gives the subtotal.
    if (subtotal >= DISCOUNT_THRESHOLD) {              // >= means "greater than OR equal to". So exactly AED 60.00 DOES get the discount.
        return round_money(subtotal * DISCOUNT_RATE); // The discount is 10% of the subtotal (multiply by 0.10), rounded.
    }
    return 0.0f;   //We only reach this line if the subtotal is under 60, so there is no discount: 0.00.
}

float calc_vat(float amount_after_discount) {              // Works out VAT. The input is called amount_after_discount on purpose: VAT is charged on the amount AFTER the discount has been taken off
    return round_money(amount_after_discount * VAT_RATE); // 5% of that amount (multiply by 0.05), rounded.
}

/*show_menu() - print the menu
prints the numbered menu with prices in AED.*/

void show_menu(void) {  //Starts the function. It takes nothing in and gives nothing back; it just prints
    int i;             //Loop counter.

    printf("\n============== %s MENU ==============\n", COMPANY_NAME);  //Prints the menu title. The %s is replaced by the business name.
    for (i = 0; i < ITEM_COUNT; i++) {        // Loop through all 10 menu items, i = 0 to 9.
        printf("   %2d. %-24s AED %6.2f\n", i + 1, item_names[i], item_prices[i]);   /*Prints one menu line. i + 1 is shown because arrays start at 0 but the menu starts at 1.
                                                                                    In the format: %2d is a whole number 2 characters wide, %-24s is text left-aligned in 24 characters,
                                                                                    and %6.2f is a decimal number 6 wide with exactly 2 digits after the point. This keeps the columns tidy.*/
    }
    printf("    0. That's all - finish the order\n");  //Prints the "0 = finish" option.
    printf("============================================\n"); //Prints the bottom line of the menu.
}

/* print_order() - show the order so far
shows the items ordered so far, the running subtotal, and how close the customer is to the discount */
void print_order(int quantities[]) {     // Starts the function. It gives the quantities array.
    int   i;                            // Loop counter.
    float subtotal = calc_subtotal(quantities);   // Calls calc_subtotal to get the total so far and stores it in subtotal.

    printf("\n  ------------ YOUR ORDER ------------\n");   // Prints the heading.
    for (i = 0; i < ITEM_COUNT; i++) {                     // Goes through all 10 items.
        if (quantities[i] > 0) {                          //Only show items that were actually ordered (quantity above 0).
            printf("   %2d x %-22s %8.2f\n", quantities[i], item_names[i],     //Prints one order line: quantity, item name, and the line total (price times quantity). The line is split in two only so it is easier to read.
                   item_prices[i] * quantities[i]);
        }
    }
    printf("  ------------------------------------\n");       // Prints a divider line.
    printf("   Subtotal so far:    AED %8.2f\n", subtotal);  //Prints the running subtotal with 2 decimals.
    if (subtotal >= DISCOUNT_THRESHOLD) {                   // Has the order reached the discount amount?
        printf("   Good news: this order gets %.0f%% off!\n\n", DISCOUNT_RATE * 100.0f); // Yes: tell the customer. %.0f prints a decimal with no digits after the point, and %% prints a real % sign. DISCOUNT_RATE * 100 turns 0.10 into 10
    } else {   // No..
        printf("   Spend AED %.2f more to get %.0f%% off.\n\n",  // so tell them how much more to spend to get the discount (the threshold minus the subtotal).
               DISCOUNT_THRESHOLD - subtotal, DISCOUNT_RATE * 100.0f);
    }
}

/*take_order() - add items until 0
In short: keeps adding items to the order until the cashier types 0 */

void take_order(int quantities[]) {    // Starts the function. Arrays are changed "in place", so when this function adds to quantities, main() sees the change too.
    int choice, quantity;             // Two boxes declared on one line: the menu number typed, and how many of it

    do {      // A do-while loop. Unlike while, it runs the body at least once and checks its condition at the bottom
        choice = ask_number_in_range("Item number (1-10), or 0 when done: ",   // Asks for a menu number between 0 and 10, using the safe question function. 0 means "I'm finished". Anything else is rejected and re-asked, which is requirement R5.
                                     0, ITEM_COUNT, "menu number");
        if (choice != 0) {                          // If they did not type 0 (!= means "is not equal to")
            quantity = ask_number_in_range("How many? (1-10): ",  // ask how many, which must be between 1 and 10.
                                           MIN_QUANTITY, MAX_QUANTITY, "quantity");
            quantities[choice - 1] += quantity;  // Stores the quantity. choice - 1 because arrays start at 0 but the menu starts at 1. += adds to whatever is already there, so ordering the same item twice adds the quantities together.
            printf("\n  Added: %d x %s\n", quantity, item_names[choice - 1]);  //Confirms what was just added.
            print_order(quantities);           // Shows the order so far, with the subtotal
        }
    } while (choice != 0);   // Goes back to the top and repeats UNTIL the cashier types 0.
}

/*reset_order() - empty the order
In short: sets every quantity back to 0, which gives an empty order */

void reset_order(int quantities[]) {     // Starts the function. Used for every new customer, and when a customer says their order is wrong.
    int i;                              // Loop counter.

    for (i = 0; i < ITEM_COUNT; i++) {    // Goes through all 10 boxes.
        quantities[i] = 0;               // Sets each box to 0, so nothing is ordered. A single = stores a value
    }
}

/* print_receipt() - the final bill
In short: prints the final bill: items, subtotal, discount, amount after discount, VAT and the total to pay.*/

void print_receipt(int order_number, const char customer_name[], char order_type,   // The function header is split over three lines so it is readable. It is given nine pieces of information: the receipt number, the customer's name, dine-in or takeaway, the quantities, and all the money amounts (subtotal, discount, amount after discount, VAT, total).
                   int quantities[], float subtotal, float discount,
                   float amount_after_discount, float vat, float total) {
    int i;          // Loop counter

    printf("\n==============================================\n");                        // Prints the top border of the receipt.
    printf("              %s\n                 FINAL BILL\n", COMPANY_NAME);            // Prints the business name and the words FINAL BILL on the next line.
    printf("   Receipt No: %03d       Served by: %s\n", order_number, CASHIER_NAME);   // Prints the receipt number and who served. %03d is a whole number padded with zeros to 3 digits, so 1 prints as 001.
    printf("   Customer:   %s (%s)\n", customer_name,              // Prints the customer's name and the order type. The part with ? and : is a short if/else. It reads: if order_type is 'd' use "Dine-in", otherwise use "Takeaway".
           order_type == 'd' ? "Dine-in" : "Takeaway");
    printf("==============================================\n");  // Border line.
    printf("%-22s %3s %8s %9s\n", "Item", "Qty", "Price", "Total");    // Prints the column headings (Item, Qty, Price, Total), lined up with the numbers below.
    printf("----------------------------------------------\n");   // Divider line.
    for (i = 0; i < ITEM_COUNT; i++) {                           // Goes through all 10 items.
        if (quantities[i] > 0) {                                // Only items that were ordered.
            printf("%-22s %3d %8.2f %9.2f\n", item_names[i], quantities[i],
                   item_prices[i], item_prices[i] * quantities[i]);                 // Prints one receipt line: name, quantity, unit price, and line total.
        }
    }
    printf("----------------------------------------------\n");                 // Divider line.
    printf("%-30s AED %8.2f\n", "Subtotal:", subtotal);                        // Prints the subtotal. %-30s is the label left-aligned in 30 characters, and %8.2f is the amount, 8 wide with exactly 2 decimals.
    if (discount > 0.0f) {                                                    // Was a discount earned?
        printf("%-30s AED %8.2f\n", "Discount (10%):", -discount);           // Yes: prints the discount as a negative number (-discount) so it reads as money taken off.
    } else {                                                                // No.
        printf("%-30s AED %8.2f\n", "Discount (none):", 0.0f);             // print "Discount (none)" with 0.00.
    }
    printf("%-30s AED %8.2f\n", "Amount after discount:", amount_after_discount);           // Prints the amount after the discount.
    printf("%-30s AED %8.2f\n", "VAT (5%):", vat);                                         // Prints the 5% VAT.
    printf("----------------------------------------------\n");                           // Divider line.
    printf("%-30s AED %8.2f\n", "TOTAL TO PAY:", total);                                 // Prints the total the customer pays.
    printf("==============================================\n");                         // Bottom border.
}

float take_cash_payment(float total) {                                              // Starts the function. It is given the total to pay and gives back the cash received.
    float cash;                                                                    // Three boxes: cash is the amount typed, result remembers whether scanf worked, and line_is_clean remembers whether the line had nothing odd after the number
    int   result;
    int   line_is_clean;

    while (1) {                                                                 // Repeat until enough valid cash is given
        printf("Cash received (AED): ");                                       // Asks for the cash amount.
        result = scanf("%f", &cash);                                          // Reads a number WITH decimals (%f means "decimal number") into cash.
        if (result == EOF) {                                                 // If the input ended, stop safely.
            input_ended();
        }
        line_is_clean = clean_line();                                      // Always clear the rest of the line, even after a failed read. Otherwise typed letters stay stuck in memory and the question repeats forever.

        if (result != 1 || !line_is_clean || !(cash >= 0.0f)) {          // Reject the amount if: scanf did not read a number, OR there was junk after it, OR it is negative. The !(cash >= 0.0f) part catches negative numbers and also the special value "nan" (not a number), because comparisons with nan are never true.
            printf("  Invalid amount! Please type a number, like 100 or 75.60.\n");   // Shows the error message. The loop will ask again.
        } else if (cash > MAX_CASH) {                                  // Otherwise, reject amounts above the AED 10,000 limit. This is checked BEFORE rounding so a huge value like "inf" cannot break the rounding
            printf("  That amount is too large. Please type up to AED %.2f.\n", MAX_CASH);    // Shows the error message.
        } else {                                                     // Otherwise the amount is a sensible number...
            cash = round_money(cash);                               // round it to 2 decimals.
            if (cash >= total) {                                   // Is it enough to pay the bill?
                return cash;                                      //Yes: hand it back and leave the function.
            }
            printf("  Not enough! The total is AED %.2f.\n", total);   // We only get here if the cash was too small, so say so. The loop asks again.
        }
    }
}

int main(void) {                                         // The start of the program. int means main gives back a whole number when it ends. (void) means nothing is passed in.
    int   quantities[ITEM_COUNT];                       // An array of 10 whole numbers: how many of each menu item the CURRENT customer has ordered.
    char  customer_name[NAME_SIZE];                    // An array of characters (text) with room for the customer's name.
    char  order_type;                                 // One letter: 'd' for dine-in or 't' for takeaway
    char  order_is_right;                            // One letter: 'y' or 'n', the answer to "Is the order correct?".
    char  serve_another;                            // One letter: 'y' or 'n', the answer to "Serve another customer?".
    int   pay_method;                              // 1 for cash or 2 for card.
    int   orders_completed = 0;                   // Counts the bills printed today. It starts at 0 and is also used as the receipt number.
    float daily_sales      = 0.0f;               // The total money taken today. It starts at 0.00.
    float subtotal, discount, amount_after_discount, vat, total, amount_paid;      //A list of decimal boxes for one order's money amounts: subtotal, discount, amount after discount, VAT, total, and the amount the customer paid.

    printf("==============================================\n");                  // Prints the welcome banner when the till starts.
    printf("   Welcome to %s - QuickBite Till\n", COMPANY_NAME);
    printf("==============================================\n");

    do {                                                                      // The OUTER loop. One lap of this loop is one customer. It runs at least once.
         reset_order(quantities);                                            // Starts each customer with an empty order.

        printf("\n------------- NEW CUSTOMER -------------\n");             // Prints a heading for the new customer.
        printf("Customer's first name: ");                                 // Asks for the customer's name.
        if (scanf("%19s", customer_name) == EOF) {                        // Reads the name. %s reads one word of text, and the 19 limits it to 19 letters so it can never overflow the 20 boxes of customer_name. There is no & here because an array's name already is its address. If the input ended, stop safely.
            input_ended();
        }
        clean_line();                                                   // Throws away anything else typed on that line (for example a second word).
        order_type = ask_two_options("Dine-in or takeaway? (d/t): ", 'd', 't');     // Asks "Dine-in or takeaway?" and only accepts d or t.

        do {                                                                     // The INNER loop: take the order, read it back, and repeat if the customer wants to start again.
            show_menu();                                                        // Shows the menu
            take_order(quantities);                                            // Takes items until the cashier types 0
            subtotal = calc_subtotal(quantities);                             // Works out the subtotal of what was ordered

            if (subtotal == 0.0f) {                                        // Was nothing ordered at all?
                order_is_right = 'y';                                     // Yes: set the answer to 'y' just so the loop ends. There is nothing to check. (Single quotes mean one character.)
            } else {                                                     // Otherwise there is an order to check
                printf("\nLet me read that back to you, %s:\n", customer_name);  // Says it is reading the order back.
                print_order(quantities);                                        // Shows the order and subtotal.
                order_is_right = ask_two_options(                              // Asks "Is the order correct?" and stores y or n.
                    "Is the order correct? (y = yes, n = start again): ", 'y', 'n');
                if (order_is_right == 'n') {                                 // If they said no...
                    reset_order(quantities);                                // ...clear the order so they can start again.
                }
            }
        } while (order_is_right == 'n');                                 // Goes round the inner loop again ONLY if they said n. Once they say y, it moves on.

        if (subtotal == 0.0f) {                                        // Was nothing ordered?
            printf("\nNo items were ordered, so there is no bill.\n");     // Yes: say there is no bill.
        } else {                                                          // Otherwise there is a bill to make.
            discount              = calc_discount(subtotal);             // The money maths, in this order. 1: work out the discount from the subtotal.
            amount_after_discount = round_money(subtotal - discount);   // 2: take it off to get the amount after discount
            vat                   = calc_vat(amount_after_discount);   // 3: work out the VAT on that smaller amount.
            total                 = round_money(amount_after_discount + vat);  // 4: add the VAT to get the total to pay. Example: 80.00, discount 8.00, 72.00, VAT 3.60, total 75.60.
            orders_completed++;                                               // ++ adds 1. This bill is the next one, so the receipt number goes up.

            print_receipt(orders_completed, customer_name, order_type, quantities,     // Prints the receipt, passing in all the information it needs
                          subtotal, discount, amount_after_discount, vat, total);     // Asks how they will pay and only accepts 1 (cash) or 2 (card).

            pay_method = ask_number_in_range("\nHow will you pay? (1 = Cash, 2 = Card): ",   // If they chose cash..
                                             PAY_CASH, PAY_CARD, "payment option");         // ..ask for the cash. This function keeps asking until it is enough.
            if (pay_method == PAY_CASH) {                                                  // Prints the cash received and the change, which is the amount paid minus the total (rounded).
                amount_paid = take_cash_payment(total);
                printf("\nCash received: AED %.2f\nChange given:  AED %.2f\n",
                       amount_paid, round_money(amount_paid - total));
            } else {                                                            // Otherwise they chose card
                printf("\nPaid by card:  AED %.2f\n", total);                  // so just say it was paid by card, for the exact total.
            }
            daily_sales += total;                                            // Adds this bill to today's sales.
            printf("\nOrder %03d is for %s - we will call your name!\n",    // Calls out the customer's name, like a real food truck.
                   orders_completed, customer_name);
        }

        serve_another = ask_two_options("\nServe another customer? (y/n): ", 'y', 'n');    // Asks if there is another customer (y or n).
    } while (serve_another == 'y');                                                       // Goes back to the top of the outer loop for the next customer if they said y. If n, the loop ends.

    printf("\n============ END OF DAY SUMMARY ============\n");                         // Prints the end-of-day heading.
    printf("Bills printed: %d     Total sales: AED %.2f\n", orders_completed, daily_sales);   // Prints how many bills were made and the total sales.
    printf("Till closed. Goodbye!\n");                                                       // Prints the goodbye message.
    return 0;                                                                               // Ends main and tells the computer the program finished without problems
}
