// SortingVisualiser.cpp : This file contains the 'main' function. The execution of the program begins and ends here.

//Including the libraries
#include<iostream>
#include "raylib.h"
#include <vector>
#include <cmath>
#include <stdio.h>
#include <string>
#include <stdlib.h>


// Default screen Width, Height and min widnow size
#define ScreenWidth 1200
#define ScreenHeight 800
#define MinWindowWidth 500
#define MinWindowHeight 800



// Piller's colors:
#define NORMAL 0
#define COMPARING 1
#define SORTED 2
#define SELECTED 3
#define MINIMUM 4
#define LEFT 6
#define RIGHT 7


//Defining FPS
#define FPS 120


// Size of Array of numbers
int NumberOfPillers = 20;


// Function Prototypes:
void ShowMenuScreen();
void ShowStartOptions();
void ShowEndingScreen();

//All Buttons
void Button(float x, float y, char* Text, Color color, bool& state);
void Start_Button(float size, float font, char Start[]);
void Selection_Sort_Button(float size, char Selection_Sort_Text[]);
void Insertion_Sort_Button(float size, char Insertion_Sort_Text[]);
void Bubble_Sort_Button(float size, char Bubble_Sort_Text[]);
void Merge_Sort_Button(float size, char Merge_Sort_Text[]);
void Shell_Sort_Button(float size, char Shell_Sort_Text[]);

Color FindColorForPiller(int pillerState);
void DrawArray(std::vector<std::pair<int, int>> arr);
void RandomizeArray(std::vector<std::pair<int, int>>& arr);

// Sorting Functions Prototypes:
void SortArray();
void Selection_Sort(std::vector<std::pair<int, int>>& arr);
void Insertion_Sort(std::vector<std::pair<int, int>>& arr);
void Bubble_Sort(std::vector<std::pair<int, int>>& arr);
void Merge_Sort(std::vector<std::pair<int, int>>& arr);
void MergeSort_Finale(std::vector<std::pair<int, int>>& arr,
    int leftIndex, int rightIndex);
void Merge(std::vector<std::pair<int, int>>& arr,
    int leftIndex, int m, int rightIndex);
void Shell_Sort(std::vector<std::pair<int, int>>& arr);


// Changing size and speed
void ChangeSize(char operation, int& value);
void ChangeSpeed(char operation, int& value);

// State :
bool ShouldShowMenuScreen = true;
bool ShouldShowStartOptions = false;
bool GameShouldStart = false;
bool GameShouldStop = false;
bool ShouldShowEndingScreen = false;
bool addSpeed = false;
bool subSpeed = false;
bool addSize = false;
bool subSize = false;
bool NormalSize = false;
bool NormalSpeed = false;
bool SelectionSortPressed = false;
bool InsertionSortPressed = false;
bool BubbleSortPressed = false;
bool MergeSortPressed = false;
bool ShellSortPressed = false;
bool ShouldRandomizeArray = true;

//Array of status of sorting buttons
std::vector<bool*> SortingChoice = {
    &SelectionSortPressed,
    &InsertionSortPressed,
    &BubbleSortPressed,
    &MergeSortPressed,
    &ShellSortPressed,
};


// Array of Values and It's States;
std::vector<std::pair<int, int>> arr(NumberOfPillers);

int SortingSpeed = 7;


//Main function
int main() {

    // Window Configuration:
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(ScreenWidth, ScreenHeight, ":: Sorting In Action ::");
    SetWindowMinSize(MinWindowWidth, MinWindowHeight);


    // Set FPS
    SetTargetFPS(FPS);



    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(VIOLET);


        if (ShouldShowMenuScreen) {
            ShowMenuScreen();
        }


        ShouldShowStartOptions = false;
        for (bool* i : SortingChoice) {
            if (*i == true)
                ShouldShowStartOptions = true;
        }


        if (ShouldShowStartOptions) {
            ShowStartOptions();
        }


        if (GameShouldStart) {
            ShouldShowMenuScreen = false;
            ShouldShowStartOptions = false;

            SetTargetFPS(SortingSpeed);
            SortArray();

            GameShouldStart = false;
            ShouldShowEndingScreen = true;
        }


        if (ShouldShowEndingScreen) {
            ShowEndingScreen();
        }

        if (ShouldRandomizeArray) {
            RandomizeArray(arr);
        }

        EndDrawing();
    }


    CloseWindow();

    return EXIT_SUCCESS;
}


//Home Screen with Sorting options
void ShowMenuScreen() {
    float font = (2.5 * GetScreenWidth()) / 100;
    char Selection_Sort_text[] = "Selection Sort!";
    float tmp = (GetScreenWidth() * 5) / 100;
    Selection_Sort_Button(tmp, Selection_Sort_text);

    char Insertion_Sort_Text[] = "Insertion Sort!";
    tmp += MeasureText(Selection_Sort_text, font) + font;
    Insertion_Sort_Button(tmp, Insertion_Sort_Text);


    char Bubble_Sort_Text[] = "Bubble Sort!";
    tmp += MeasureText(Insertion_Sort_Text, font) + font;
    Bubble_Sort_Button(tmp, Bubble_Sort_Text);


    char Merge_Sort_Text[] = "Merge Sort!";
    tmp += MeasureText(Bubble_Sort_Text, font) + font;
    Merge_Sort_Button(tmp, Merge_Sort_Text);

    char Shell_Sort_text[] = "Shell Sort!";
    tmp += MeasureText(Merge_Sort_Text, font) + font;
    Shell_Sort_Button(tmp, Shell_Sort_text);

    DrawArray(arr);
}


//Scrren that contains various operations on sorting
void ShowStartOptions() {
    float font = (2.5 * GetScreenWidth()) / 100;

    char StartText[] = "Start Sorting!";
    float tmp = (5 * GetScreenWidth()) / 100;
    Start_Button(tmp, font, StartText);

    char RandomizeArrayText[] = "New Array!";
    Button(tmp, 50 + GetScreenHeight() / 20 + font * 2,
        RandomizeArrayText, GREEN, ShouldRandomizeArray);


    addSpeed = false;
    subSpeed = false;
    addSize = false;
    subSize = false;
    NormalSize = false;
    NormalSpeed = false;

    tmp = (85 * GetScreenWidth()) / 100;
    char TimeButtonText[] = "Speed";


    Button(tmp, 50 + GetScreenHeight() / 20 + font * 2,
        TimeButtonText, GREEN, NormalSpeed);


    if (NormalSpeed) {
        ChangeSpeed('/', SortingSpeed);
        return;
    }


    tmp += MeasureText(TimeButtonText, font) + 20;
    char SpeedPlusButtonText[] = "+";
    Button(tmp, 50 + GetScreenHeight() / 20 + font * 2,
        SpeedPlusButtonText, WHITE, addSpeed);


    if (addSpeed) {
        ChangeSpeed('+', SortingSpeed);
        return;
    }

    tmp += MeasureText(SpeedPlusButtonText, font) + 20;
    char SpeedMinusButtonText[] = "-";
    Button(tmp, 50 + GetScreenHeight() / 20 + font * 2,
        SpeedMinusButtonText, WHITE, subSpeed);


    if (subSpeed) {
        ChangeSpeed('-', SortingSpeed);
        return;
    }


    tmp = (85 * GetScreenWidth()) / 100;
    char SizeButtonText[] = "Size";
    Button(tmp, GetScreenHeight() / 20 + font * 2,
        SizeButtonText, GREEN, NormalSize);


    if (NormalSize) {
        ChangeSize('/', NumberOfPillers);
        return;
    }



    tmp += MeasureText(SizeButtonText, font) + 20;
    char SizePlusButton[] = "+";
    Button(tmp, GetScreenHeight() / 20 + font * 2,
        SizePlusButton, WHITE, addSize);

    if (addSize) {
        ChangeSize('+', NumberOfPillers);
        return;
    }

    tmp += MeasureText(SizePlusButton, font) + 20;
    char SizeMinusButton[] = "-";
    Button(tmp, GetScreenHeight() / 20 + font * 2,
        SizeMinusButton, WHITE, subSize);


    if (subSize) {
        ChangeSize('-', NumberOfPillers);
        return;
    }
}



//Function for changing array size
void ChangeSize(char operation, int& value) {
    switch (operation)
    {
    case '-':
        if (value > 5) {
            value -= 5;
            for (int i = 0; i < 5; i++)
                arr.pop_back();
        }
        break;

    case '+':
        value += 5;
        for (int i = 0; i < 5; i++)
            arr.push_back({ GetRandomValue(40, MinWindowHeight - 200), NORMAL });
        break;


    default:
        while (NumberOfPillers > 20) {
            NumberOfPillers--;
            arr.pop_back();
        }

        while (NumberOfPillers < 20) {
            NumberOfPillers++;
            arr.push_back({ GetRandomValue(40, MinWindowHeight - 200), NORMAL });
        }
        break;
    }

    for (int i = 0; i < NumberOfPillers; i++) {
        arr[i].second = NORMAL;
    }


    DrawArray(arr);
}


//Function for changing sorting speed
void ChangeSpeed(char operation, int& value) {

    switch (operation)
    {
    case '-':
        value = value - 10 > 0 ? value - 10 : value;
        break;

    case '+':
        value += 10;
        break;

    default:
        value = 11;
        break;

    }
}


//
void ShowEndingScreen() {
    DrawArray(arr);

    ShouldShowMenuScreen = true;
    ShouldShowEndingScreen = false;
}


//Defining Various Buttons

void Start_Button(float size, float font, char Start[]) {
    Button(size, GetScreenHeight() / 20 + font * 2,
        Start, GREEN, GameShouldStart);

    return;
}


void Selection_Sort_Button(float size, char Selection_Sort_Text[]) {
    Color color;
    if (SelectionSortPressed)
        color = GREEN;
    else
        color = WHITE;

    Button(size, GetScreenHeight() / 20, Selection_Sort_Text, color,
        SelectionSortPressed);
}

void Insertion_Sort_Button(float size, char Insertion_Sort_Text[]) {
    Color color;
    if (InsertionSortPressed)
        color = GREEN;
    else
        color = WHITE;

    Button(size, GetScreenHeight() / 20, Insertion_Sort_Text, color,
        InsertionSortPressed);
}

void Bubble_Sort_Button(float size, char Bubble_Sort_Text[]) {
    Color color;
    if (BubbleSortPressed)
        color = GREEN;
    else
        color = WHITE;

    Button(size, GetScreenHeight() / 20, Bubble_Sort_Text, color,
        BubbleSortPressed);
}

void Merge_Sort_Button(float size, char Merge_Sort_Text[]) {
    Color color;
    if (MergeSortPressed)
        color = GREEN;
    else
        color = WHITE;

    Button(size, GetScreenHeight() / 20, Merge_Sort_Text, color,
        MergeSortPressed);
}

void Shell_Sort_Button(float size, char Shell_Sort_Text[]) {
    Color color;
    if (ShellSortPressed)
        color = GREEN;
    else
        color = WHITE;

    Button(size, GetScreenHeight() / 20, Shell_Sort_Text, color,
        ShellSortPressed);
}


//Defining button and its function
void Button(float x, float y, char* Text, Color color, bool& state) {
    float font = (2.5 * GetScreenWidth()) / 100;
    Rectangle r1;
    r1.x = x;
    r1.y = y;
    r1.width = (float)MeasureText(Text, font);
    r1.height = (float)font;


    if (CheckCollisionPointRec(GetMousePosition(), r1)) {

        DrawText(Text, x, y, font, ORANGE);

        if (IsMouseButtonPressed(0)) {
            if (state == true)
                state = false;
            else
                state = true;


            if (state != ShouldRandomizeArray && state != GameShouldStart
                && (state != addSize && state != subSize && state != addSpeed
                    && state != subSpeed && state != NormalSize && state != NormalSpeed))

                for (bool* i : SortingChoice)
                    if (i != &state)
                        *i = false;
            return;
        }
    }
    else {
        DrawText(Text, x, y, font, color);
    }

    return;
}

//Converting the array into pillars
void DrawArray(std::vector<std::pair<int, int>> arr) {
    // Drawing the array / pillers:
    float BarWidth = (float)GetScreenWidth() / NumberOfPillers;


    for (int i = 0; i < NumberOfPillers; i++) {
        Color color = FindColorForPiller(arr[i].second);

        DrawRectangleV(Vector2{ (float)i * BarWidth,
            (float)GetScreenHeight() - arr[i].first },
            Vector2{ BarWidth, (float)arr[i].first }, color);
        DrawRectangleLines(
            (int)((float)i * BarWidth) - 1, (int)((float)GetScreenHeight() - arr[i].first) - 1,
            BarWidth + 2, (int)arr[i].first + 2,
            BLACK  // Color of the outline
        );

        // Assuming arr[i].first is an integer
        int integerValue = arr[i].first;

        // Convert the integer to a string using std::to_string()
        std::string stringValue = std::to_string(integerValue);

        // Use c_str() to get the const char* for DrawText
        DrawText(stringValue.c_str(), (int)((float)i * BarWidth) + i / 6 + BarWidth / 6, (int)((float)GetScreenHeight() - arr[i].first) - 1, (1.8 * GetScreenWidth()) / 100, BLACK);

    }
}


// Generating a random array
void RandomizeArray(std::vector<std::pair<int, int >>& arr) {
    if (ShouldRandomizeArray) {
        for (int i = 0; i < NumberOfPillers; i++)
            arr[i] = { GetRandomValue(40, MinWindowHeight - 200), NORMAL };

        ShouldRandomizeArray = false;
    }
}


//Defing the color for pillars at different states
Color FindColorForPiller(int pillerState) {
    switch (pillerState)
    {
    case SELECTED:
        return RED;
        break;

    case COMPARING:
        return ORANGE;
        break;

    case SORTED:
        return GREEN;
        break;

    case MINIMUM:
        return GOLD;
        break;


    case LEFT:
        return GOLD;
        break;

    case RIGHT:
        return BLUE;
        break;


    default:
        return LIGHTGRAY;
        break;
    }
}

//Selecting which type of sorting should be done
void SortArray() {
    for (bool* state : SortingChoice) {
        if (*state == true) {
            if (state == &InsertionSortPressed)
                Insertion_Sort(arr);

            else if (state == &SelectionSortPressed)
                Selection_Sort(arr);

            else if (state == &BubbleSortPressed)
                Bubble_Sort(arr);

            else if (state == &MergeSortPressed)
                Merge_Sort(arr);
            else if (state == &ShellSortPressed)
                Shell_Sort(arr);

        }

        *state = false;
    }


    return;
}



// Selection Sort:
// Selection Sort algorithm applied to a vector of pairs representing elements and their states (e.g., SELECTED, COMPARING, MINIMUM, SORTED, NORMAL).

void Selection_Sort(std::vector<std::pair<int, int>>& arr) {

    // Loop through the array
    for (int i = 0; i < NumberOfPillers; i++) {
        arr[i].second = SELECTED; // Mark the current element as SELECTED.

        // Initialize the minimum number as the current element
        std::pair<int, int> minNum = { arr[i].first, i };

        // Loop through the array starting from the current index to find the minimum element
        for (int j = i; j < NumberOfPillers; j++) {
            arr[j].second = COMPARING; // Mark the element as currently being compared.
            arr[minNum.second].second = MINIMUM; // Mark the previously known minimum element as MINIMUM.

            // Compare if the current element is smaller than the minimum element found so far
            if (minNum.first > arr[j].first) {

                // Update the visualization states for the elements being compared
                if (minNum.second == i)
                    arr[minNum.second].second = SELECTED;
                else if (minNum.second == j)
                    arr[minNum.second].second = COMPARING;
                else
                    arr[minNum.second].second = NORMAL;

                // Update the minimum number if a smaller element is found
                minNum = { arr[j].first, j };
            }

            // Visualization code using Raylib (drawing the current state of the array)
            BeginDrawing();
            ClearBackground(VIOLET);
            DrawArray(arr); // Function to visualize the array.
            // Update the states of elements in the visualization
            if (j == i)
                arr[j].second = SELECTED;
            else
                arr[j].second = NORMAL;
            EndDrawing();
        }

        // Swap the current element with the minimum element found
        std::swap(arr[i], arr[minNum.second]);
        arr[i].second = SORTED; // Mark the current element as SORTED.

        // Reset the states for elements after the current index for the next iteration
        for (int j = i + 1; j < NumberOfPillers; j++) {
            arr[j].second = NORMAL; // Mark elements as NORMAL.
        }
    }
}



// Insertion Sort algorithm applied to a vector of pairs representing elements and their states (e.g., SELECTED, COMPARING, SORTED, NORMAL).

void Insertion_Sort(std::vector<std::pair<int, int>>& arr) {

    // arr[0].second = SORTED; // Mark the first element as sorted.

     // Loop through the array starting from the second element.
    for (int i = 1; i < arr.size(); i++) {
        BeginDrawing(); // Begin the visualization.

        ClearBackground(VIOLET); // Clear the background.

        arr[i].second = SELECTED; // Mark the current element as SELECTED for comparison.

        //Color color = GREEN; // Define a color (not used in the provided code).

        int key = arr[i].first; // Store the current element's value as 'key'.

        // Sorting the array
        int tmp = 0;
        int j = i - 1;
        arr[j].second = COMPARING;
        // Perform the insertion sort by shifting elements to the right until the correct position for 'key' is found.
        while (j >= 0 && arr[j].first > key) {

            // Swap the current and previous elements in the array to shift 'key' to the correct position.
            //std::swap(arr[j + 1].first, arr[j].first);
            arr[j].second = COMPARING;
            arr[j + 1].first = arr[j].first;
            // Make the comparing pillar.


            BeginDrawing();
            ClearBackground(VIOLET);
            DrawArray(arr);

            // Make the Selected pillar sorted again (it already was).
            arr[j].second = NORMAL;

            EndDrawing();

            j--;
            tmp++;
        }

        if (tmp == 0) {
            BeginDrawing();
            ClearBackground(VIOLET);
            DrawArray(arr);
            EndDrawing();
        }

        // DE-Select the unnecessary pillars.
        for (int k = 0; k < NumberOfPillers; k++)
            if (arr[k].second == SELECTED)
                arr[k].second = NORMAL;

        // Color the sorted pillars.
        //for (int k = i - 1; k >= 0; k--)
          //  arr[k].second = SORTED;

        // Place 'key' in its correct sorted position.
        arr[j + 1].first = key;
        arr[j + 1].second = NORMAL;

        EndDrawing(); // End the visualization.
    }
    for (int k = NumberOfPillers - 1; k >= 0; k--)
        arr[k].second = SORTED;
    //arr[NumberOfPillers - 1].second = SORTED; // Mark the last element as sorted.
}



// Bubble Sort:
// Bubble Sort algorithm applied to a vector of pairs representing elements and their states (e.g., SELECTED, SORTED, NORMAL).

void Bubble_Sort(std::vector<std::pair<int, int>>& arr) {

    int endingPoint = NumberOfPillers; // Variable to mark the ending point of the unsorted part of the array.

    bool swapped; // Boolean variable to track if any elements were swapped during a pass through the array.
    do {
        swapped = false; // Reset the swap flag before each pass through the array.

        for (int i = 0; i < endingPoint - 1; i++) {
            arr[i].second = SELECTED; // Mark the current element and the next element as selected for comparison in visualization.
            arr[i + 1].second = COMPARING;

            if (arr[i] > arr[i + 1]) { // Compare adjacent elements and if they are in the wrong order, swap them.
                std::swap(arr[i], arr[i + 1]); // Swap the elements.
                swapped = true; // Set the flag to true to indicate that a swap occurred.
            }

            BeginDrawing(); // Begin the visualization.

            ClearBackground(VIOLET); // Clear the background.

            // Mark elements as SORTED after the ending point in the array for visualization.
            for (int k = NumberOfPillers - 1; k >= endingPoint; k--)
                arr[k].second = SORTED;

            DrawArray(arr); // Visualize the current state of the array.

            // Mark elements as NORMAL up to the current index for visualization.
            for (int k = i; k >= 0; k--)
                arr[k].second = NORMAL;

            EndDrawing(); // End the visualization.

        }

        endingPoint--; // Move the ending point of the unsorted part of the array towards the beginning.

    } while (swapped); // Repeat the process until no more swaps are performed.

    // Finally, mark all elements as SORTED after the sorting is completed.
    for (int k = NumberOfPillers - 1; k >= 0; k--)
        arr[k].second = SORTED;
}



// Merge Sort algorithm applied to a vector of pairs representing elements and their states (e.g., SELECTED, COMPARING, LEFT,RIGHT  SORTED, NORMAL).
// TODO;
void Merge_Sort(std::vector<std::pair<int, int>>& arr) {
    MergeSort_Finale(arr, 0, NumberOfPillers - 1);
}

void MergeSort_Finale(std::vector<std::pair<int, int>>& arr,
    int leftIndex, int rightIndex) {

    if (leftIndex >= rightIndex)
        return;

    int m = (leftIndex + rightIndex) / 2;

    MergeSort_Finale(arr, leftIndex, m);
    MergeSort_Finale(arr, m + 1, rightIndex);

    Merge(arr, leftIndex, m, rightIndex);
}

void Merge(std::vector<std::pair<int, int>>& arr,
    int leftIndex, int m, int rightIndex) {

    int arrSizeLeft = m - leftIndex + 1;
    int arrSizeRight = rightIndex - m;



    //int left[arrSizeLeft];
    std::vector<int> left(arrSizeLeft);
    for (int i = 0; i < arrSizeLeft; i++) {
        left[i] = arr[leftIndex + i].first;
        arr[leftIndex + i].second = LEFT;
    }


    //int right[arrSizeRight];
    std::vector<int> right(arrSizeRight);
    for (int i = 0; i < arrSizeRight; i++) {
        right[i] = arr[m + 1 + i].first;
        arr[m + 1 + i].second = RIGHT;
    }



    BeginDrawing();
    ClearBackground(VIOLET);


    DrawArray(arr);

    EndDrawing();





    int i = 0; // Left index
    int j = 0; // Right index

    int k = leftIndex; // New array index;


    while (i < arrSizeLeft && j < arrSizeRight) {
        if (left[i] <= right[j]) {
            arr[k].first = left[i];
            i++;
        }
        else {
            arr[k].first = right[j];
            j++;
        }
        arr[k].second = SORTED;

        k++;
    }


    while (i < arrSizeLeft) {
        arr[k].first = left[i];
        arr[k].second = SORTED;

        i++;
        k++;
    }


    while (j < arrSizeRight) {
        arr[k].first = right[j];
        arr[k].second = SORTED;

        j++;
        k++;
    }
}



//Shell Sort algorithm applied to a vector of pairs representing elements and their states (e.g., SELECTED, COMPARING, SORTED, NORMAL).
void Shell_Sort(std::vector<std::pair<int, int>>& arr) {
    int n = arr.size();

    for (int gap = n / 2; gap > 0;) {
        bool swapped = false;

        for (int i = 0; i < n - gap; i += 1) {
            int j = i + gap;

            // Update visualization states
            arr[i].second = SELECTED;
            arr[j].second = COMPARING;

            // Visualization code using Raylib
            BeginDrawing();
            ClearBackground(VIOLET);
            DrawArray(arr); // Adjust the width as needed
            EndDrawing();

            // Compare if the current element is smaller than the element with the gap
            if (arr[i].first > arr[j].first) {
                // Swap the elements
                std::swap(arr[i], arr[j]);
                swapped = true;
            }

            // Update visualization states after the comparison
            arr[i].second = NORMAL;
            arr[j].second = NORMAL;

            // Visualization code using Raylib
            BeginDrawing();
            ClearBackground(VIOLET);
            DrawArray(arr); // Adjust the width as needed
            EndDrawing();
        }

        // If there was any swap in the inner loop, the gap remains the same
        if (swapped) {
            // Visualization code using Raylib
            BeginDrawing();
            ClearBackground(VIOLET);
            DrawArray(arr); // Adjust the width as needed
            EndDrawing();
        }
        else {
            // Otherwise, reduce the gap
            gap /= 2;
        }
    }

    // Mark all elements as sorted
    for (auto& pair : arr) {
        pair.second = SORTED;
    }

    // Final visualization
    BeginDrawing();
    ClearBackground(VIOLET);
    DrawArray(arr); // Adjust the width as needed
    EndDrawing();
}

