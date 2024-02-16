#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>


// include the user library "core" so we can use those functions
#include "core.h"
// include the user library "clinic" where the function prototypes are declared
#include "clinic.h"
#include <string.h>

// Display's the patient table header (table format)
void displayPatientTableHeader(void)
{
    printf("Pat.# Name            Phone#\n"
           "----- --------------- --------------------\n");
}

// Displays a single patient record in FMT_FORM | FMT_TABLE format
void displayPatientData(const struct Patient* patient, int fmt)
{
    if (fmt == FMT_FORM)
    {
        printf("Name  : %s\n"
               "Number: %05d\n"
               "Phone : ", patient->name, patient->patientNumber);
        displayFormattedPhone(patient->phone.number);
        printf(" (%s)\n", patient->phone.description);
    }
    else
    {
        printf("%05d %-15s ", patient->patientNumber,
               patient->name);
        displayFormattedPhone(patient->phone.number);
        printf(" (%s)\n", patient->phone.description);
    }
}

// Display's appointment schedule headers (date-specific or all records)
void displayScheduleTableHeader(const struct Date* date, int isAllRecords)
{
    printf("Clinic Appointments for the Date: ");

    if (isAllRecords)
    {
        printf("<ALL>\n\n");
        printf("Date       Time  Pat.# Name            Phone#\n"
               "---------- ----- ----- --------------- --------------------\n");
    }
    else
    {
        printf("%04d-%02d-%02d\n\n", date->year, date->month, date->day);
        printf("Time  Pat.# Name            Phone#\n"
               "----- ----- --------------- --------------------\n");
    }
}

// Display a single appointment record with patient info. in tabular format
void displayScheduleData(const struct Patient* patient,
                         const struct Appointment* appoint,
                         int includeDateField)
{
    if (includeDateField)
    {
        printf("%04d-%02d-%02d ", appoint->date.year, appoint->date.month,
               appoint->date.day);
    }
    printf("%02d:%02d %05d %-15s ", appoint->time.hour, appoint->time.min,
           patient->patientNumber, patient->name);

    displayFormattedPhone(patient->phone.number);

    printf(" (%s)\n", patient->phone.description);
}

// main menu
void menuMain(struct ClinicData* data)
{
    int selection;

    do {
        printf("Veterinary Clinic System\n"
               "=========================\n"
               "1) PATIENT     Management\n"
               "2) APPOINTMENT Management\n"
               "-------------------------\n"
               "0) Exit System\n"
               "-------------------------\n"
               "Selection: ");
        selection = inputIntRange(0, 2);
        putchar('\n');
        switch (selection)
        {
        case 0:
            printf("Are you sure you want to exit? (y|n): ");
            selection = !(inputCharOption("yn") == 'y');
            putchar('\n');
            if (!selection)
            {
                printf("Exiting system... Goodbye.\n\n");
            }
            break;
        case 1:
            menuPatient(data->patients, data->maxPatient);
            break;
        case 2:
            menuAppointment(data);
            break;
        }
    } while (selection);
}

// Menu: Patient Management
void menuPatient(struct Patient patient[], int max)
{
    int selection;

    do {
        printf("Patient Management\n"
               "=========================\n"
               "1) VIEW   Patient Data\n"
               "2) SEARCH Patients\n"
               "3) ADD    Patient\n"
               "4) EDIT   Patient\n"
               "5) REMOVE Patient\n"
               "-------------------------\n"
               "0) Previous menu\n"
               "-------------------------\n"
               "Selection: ");
        selection = inputIntRange(0, 5);
        putchar('\n');
        switch (selection)
        {
        case 1:
            displayAllPatients(patient, max, FMT_TABLE);
            suspend();
            break;
        case 2:
            searchPatientData(patient, max);
            break;
        case 3:
            addPatient(patient, max);
            break;
        case 4:
            editPatient(patient, max);
            break;
        case 5:
            removePatient(patient, max);
            break;
        }
    } while (selection);
}

// Menu: Patient edit
void menuPatientEdit(struct Patient* patient)
{
    int selection;

    do {
        printf("Edit Patient (%05d)\n"
               "=========================\n"
               "1) NAME : %s\n"
               "2) PHONE: ", patient->patientNumber, patient->name);

        displayFormattedPhone(patient->phone.number);

        printf("\n"
               "-------------------------\n"
               "0) Previous menu\n"
               "-------------------------\n"
               "Selection: ");
        selection = inputIntRange(0, 2);
        putchar('\n');

        if (selection == 1)
        {
            printf("Name  : ");
            inputCString(patient->name, 1, NAME_LEN);
            putchar('\n');
            printf("Patient record updated!\n\n");
        }
        else if (selection == 2)
        {
            inputPhoneData(&patient->phone);
            putchar('\n');
            printf("Patient record updated!\n\n");
        }

    } while (selection);
}


// Menu: Appointment Management
void menuAppointment(struct ClinicData* data)
{
    int selection;

    do {
        printf("Appointment Management\n"
               "==============================\n"
               "1) VIEW   ALL Appointments\n"
               "2) VIEW   Appointments by DATE\n"
               "3) ADD    Appointment\n"
               "4) REMOVE Appointment\n"
               "------------------------------\n"
               "0) Previous menu\n"
               "------------------------------\n"
               "Selection: ");
        selection = inputIntRange(0, 4);
        putchar('\n');
        switch (selection)
        {
        case 1:
            viewAllAppointments(data); 
            suspend();
            break;
        case 2:
            viewAppointmentSchedule(data); 
            suspend();
            break;
        case 3:
            addAppointment(data->appointments, data->maxAppointments,
                           data->patients, data->maxPatient); 
            suspend();
            break;
        case 4:
            removeAppointment(data->appointments, data->maxAppointments,
                              data->patients, data->maxPatient); 
            suspend();
            break;
        }
    } while (selection);
}

// Display's all patient data in the FMT_FORM | FMT_TABLE format
void displayAllPatients(const struct Patient patient[], int max, int fmt)
{
    int i;
    displayPatientTableHeader();
    for (i = 0; i < max; i++)
    {
        if (patient[i].patientNumber > 0)
        {
            displayPatientData(&patient[i], fmt);
        }
    }
    printf("\n");
}


// Search for a patient record based on patient number or phone number
void searchPatientData(const struct Patient patient[], int max)
{
    int selection;

    do
    {
        printf("Search Options\n"
            "==========================\n"
            "1) By patient number\n"
            "2) By phone number\n"
            "..........................\n"
            "0) Previous menu\n"
            "..........................\n"
            "Selection: ");
        selection = inputIntRange(0, 2);
        putchar('\n');
        switch (selection)
        {
        case 1:
            searchPatientByPatientNumber(patient, max);
            suspend();
            break;
        case 2:
            searchPatientByPhoneNumber(patient, max);
            suspend();
            break;
        }
    } while (selection);
}


// Add a new patient record to the patient array
void addPatient(struct Patient patient[], int max)
{
    int i = 0;

    do
    {
        i++;
    } while (patient[i].patientNumber > 0);

    if (i > max - 1)
    {
        printf("ERROR: Patient listing is FULL!\n");
        printf("\n");
        suspend();
    }
    else
    {
        patient[i].patientNumber = nextPatientNumber(patient, max);
        inputPatient(&patient[i]);
        printf("\n");
        printf("*** New patient record added ***\n");
        printf("\n");
        suspend();
    }
}

// Edit a patient record from the patient array
void editPatient(struct Patient patient[], int max)
{
    int patientNum; // patient number
    int i;

    printf("Enter the patient number: ");
    patientNum = inputInt();
    i = findPatientIndexByPatientNum(patientNum, patient, max);
    printf("\n");

    if (i > 0)
    {
        menuPatientEdit(&patient[i]);
        //printf("\n");
    }
    else
    {
        printf("ERROR: Patient record not found!\n");
        clearInputBuffer();
        printf("\n");
    }
}

// Remove a patient record from the patient array
void removePatient(struct Patient patient[], int max)
{
    int pn; // patient number
    int i = 0;
    char selection = 0;

    //struct Patient empty = { 0 };

    printf("Enter the patient number: ");
    pn = inputIntPositive();
    printf("\n");

    i = findPatientIndexByPatientNum(pn, patient, max);

    if (i != -1)
    {
        displayPatientData(&patient[i], FMT_FORM);
        printf("\n");
        printf("Are you sure you want to remove this patient record? (y/n): ");
        selection = inputCharOption("yn");
    }
    else
    {
        printf("ERROR: Patient record not found!\n");
        printf("\n");
        suspend();
        //clearInputBuffer();
    }

    if (selection == 'y')
    {
        //patient[i] = empty;

        //f = 1;
        //while (f)
        //{
        //    if (i < max)
        //    {
        //        patient[i].patientNumber = patient[i + 1].patientNumber;
        //        strcpy(patient[i].name, patient[i + 1].name);
        //        strcpy(patient[i].phone.description, patient[i + 1].phone.description);
        //        strcpy(patient[i].phone.number, patient[i + 1].phone.number);

        //        i++;
        //    }
        //    else
        //    {
        //        patient[i].patientNumber = 0;
        //        strcpy(patient[i].name, "");
        //        strcpy(patient[i].phone.description, "");
        //        strcpy(patient[i].phone.number, "");

        //        f = 0;
        //    }
        //}

        patient[i].patientNumber = 0;
        strcpy(patient[i].name, "");
        strcpy(patient[i].phone.description, "");
        strcpy(patient[i].phone.number, "");

        printf("Patient record has been removed!\n\n");
        suspend();
    }
    else if (selection == 'n')
    {
        printf("Operation aborted.\n\n");
        suspend();
    }

}

 //View ALL scheduled appointments
void viewAllAppointments(struct ClinicData *data)
{
    int i = 0, j = 0;
    int A = 1; // isAllRecords
    displayScheduleTableHeader(&data->appointments->date, A);

    sortOrderByDate(data->appointments, data->maxAppointments);

    for (i = 0; i < 50; i++)
    {
        for (j = 0; j < 20; j++)
        {
            if (data->appointments[i].patientNum == data->patients[j].patientNumber)
            {
                displayScheduleData(&data->patients[j], &data->appointments[i], A);
            }
        }
    }
    printf("\n");
}


// View appointment schedule for the user input date
void viewAppointmentSchedule(struct ClinicData *data)
{
    int year = 0, month = 0, day = 0, ly = 0, maxDay = 0;
    int A = 0, i = 0, j = 0, a = 0, p = 0, o = 1;

    printf("Year        : ");
    year = inputIntPositive();

    ly = leafYear(year);

    printf("Month (1-12): ");
    month = inputIntRange(1, 12);

    if (month == 2)
    {
        if (ly)
        {
            maxDay = 29;
        }
        else
        {
            maxDay = 28;
        }
    }
    else if (month == 4 || month == 6 || month == 9 || month == 11)
    {
        maxDay = 30;
    }
    else
    {
        maxDay = 31;
    }

    printf("Day (1-%d)  : ", maxDay);
    day = inputIntRange(1, maxDay);
    printf("\n");

    for (i = 0; i < data->maxAppointments; i++)
    {
        if (year == data->appointments[i].date.year)
        {
            if (month == data->appointments[i].date.month)
            {
                if (day == data->appointments[i].date.day)
                {
                    a = i;
                    if (o)
                    {
                        displayScheduleTableHeader(&data->appointments[a].date, A);
                        o = 0;
                    }


                    for (j = 0; j < data->maxPatient; j++)
                    {
                        if (data->appointments[a].patientNum == data->patients[j].patientNumber)
                        {
                            p = j;
                            sortOrderByDate(data->appointments, data->maxAppointments);
                            displayScheduleData(&data->patients[p], &data->appointments[a], A);
                        }
                    }
                }
            }
        }
    }

    printf("\n");
}

// Add an appointment record to the appointment array
void addAppointment(struct Appointment* appoint, int maxAppoint, struct Patient patient[], int maxPatient)
{
    int pn = 0; // patient number
    int year = 0, month = 0, day = 0, hour = 0, min = 0, maxDay = 0, ly = 0, i, reserv = 0, newNum = 0;
    int f = 1;

    for (i = 0; i < maxAppoint; i++)
    {
        if (appoint[i].patientNum <= 0)
        {
            newNum = i;
        }
    }

    printf("Patient Number: ");
    pn = inputIntPositive();

    do
    {
        reserv = 0;
        printf("Year        : ");
        year = inputIntPositive();

        ly = leafYear(year);

        printf("Month (1-12): ");
        month = inputIntRange(1, 12);

        if (month == 2)
        {
            if (ly)
            {
                maxDay = 29;
            }
            else
            {
                maxDay = 28;
            }
        }
        else if (month == 4 || month == 6 || month == 9 || month == 11)
        {
            maxDay = 30;
        }
        else
        {
            maxDay = 31;
        }

        printf("Day (1-%d)  : ", maxDay);
        day = inputIntRange(1, maxDay);

        printf("Hour (0-23)  : ");
        hour = inputIntRange(0, 23);

        printf("Minute (0-59): ");
        min = inputIntRange(0, 59);

        for (i = 0; i < maxAppoint; i++)
        {
            if (year == appoint[i].date.year)
            {
                if (month == appoint[i].date.month)
                {
                    if (day == appoint[i].date.day)
                    {
                        if (hour == appoint[i].time.hour)
                        {
                            if (min == appoint[i].time.min)
                            {
                                reserv = 1;
                            }
                        }
                    }
                }
            }
        }

        if (reserv)
        {
            printf("\nERROR: Appointment timeslot is not available!\n\n");
        }
        else if (min % MIN_INTERVAL != 0 || hour > END_HOUR || hour < START_HOUR)
        {
            while (min % MIN_INTERVAL != 0 || hour > END_HOUR || hour < START_HOUR || (hour == 14 && min > 0))
            {
                printf("ERROR: Time must be between %d:00 and %d:00 in %d minute intervals.\n\n", START_HOUR, END_HOUR, MIN_INTERVAL);

                printf("Hour (0-23)  : ");
                hour = inputIntRange(0, 23);

                printf("Minute (0-59): ");
                min = inputIntRange(0, 59);
            }
            appoint[newNum].patientNum = pn;
            appoint[newNum].date.year = year;
            appoint[newNum].date.month = month;
            appoint[newNum].date.day = day;
            appoint[newNum].time.hour = hour;
            appoint[newNum].time.min = min;
            f = 0;

            printf("\n*** Appointment scheduled! ***\n\n");
        }
    } while (f);
}

// Remove an appointment record from the appointment array
void removeAppointment(struct Appointment* appoint, int maxAppoint, struct Patient patient[], int maxPatient)
{
    int pn = 0; // patient number
    int year = 0, month = 0, day = 0, maxDay = 0, ly = 0, i = 0, reservNum = 0, petNum = 0, selection = 0;
    int e = 1;

    printf("Patient Number: ");
    pn = inputIntPositive();

    for (i = 0; i < maxPatient; i++)
    {
        if (patient[i].patientNumber == pn)
        {
            printf("Year        : ");
            year = inputIntPositive();

            ly = leafYear(year);

            printf("Month (1-12): ");
            month = inputIntRange(1, 12);

            if (month == 2)
            {
                if (ly)
                {
                    maxDay = 29;
                }
                else
                {
                    maxDay = 28;
                }
            }
            else if (month == 4 || month == 6 || month == 9 || month == 11)
            {
                maxDay = 30;
            }
            else
            {
                maxDay = 31;
            }

            printf("Day (1-%d)  : ", maxDay);
            day = inputIntRange(1, maxDay);
            printf("\n");

            for (i = 0; i < maxAppoint; i++)
            {
                if (appoint[i].date.year == year)
                {
                    if (appoint[i].date.month == month)
                    {
                        if (appoint[i].date.day == day)
                        {
                            if (pn == appoint[i].patientNum)
                            {
                                reservNum = i;
                            }

                        }
                    }
                }
            }

            for (i = 0; i < maxPatient; i++)
            {
                if (patient[i].patientNumber == pn)
                {
                    petNum = i;
                }
            }

            displayPatientData(&patient[petNum], FMT_FORM);

            printf("Are you sure you want to remove this appointment (y,n): ");
            selection = inputCharOption("yn");
            printf("\n");

            if (selection == 'y')
            {
                appoint[reservNum].patientNum = 0;
                appoint[reservNum].date.year = 0;
                appoint[reservNum].date.month = 0;
                appoint[reservNum].date.day = 0;
                appoint[reservNum].time.hour = 0;
                appoint[reservNum].time.min = 0;

                printf("Appointment record has been removed!\n\n");
            }
            else if (selection == 'n')
            {
                printf("Operation aborted.\n\n");
            }

            e = 1;
        }
        else
        {
            e = 0;
        }
    }

    if (!e)
    {
        printf("ERROR: Patient record not found!\n\n");
    }

}

// Search and display patient record by patient number (form)
void searchPatientByPatientNumber(const struct Patient patient[], int max)
{
    int patientNum = 0, i = 0, n = 0;

    printf("Search by patient number: ");
    patientNum = inputInt();

    printf("\n");


    for (i = 0; i <= max; i++)
    {
        if (patientNum == patient[i].patientNumber)
        {
            displayPatientData(&patient[i], FMT_FORM);
            n = i;
        }
    }

    if (n == 0)
    {
        printf("*** No records found ***\n");
    }

    printf("\n");
}

// Search and display patient records by phone number (tabular)
void searchPatientByPhoneNumber(const struct Patient patient[], int max)
{
    int i, n = 0;
    int fmt = 2;
    char phoneNum[PHONE_LEN + 1] = { 0 };

    phoneNum[0] = '\0';

    printf("Search by phone number: ");
    inputCString(phoneNum, PHONE_LEN, PHONE_LEN);

    printf("\n");
    displayPatientTableHeader();
    for (i = 0; i <= max; i++)
    {
        if (!strcmp(phoneNum, patient[i].phone.number))
        {
            displayPatientData(&patient[i], fmt);
            n++;
        }
    }

    if (n == 0)
    {
        printf("\n");
        printf("*** No records found ***\n");
    }
    printf("\n");
}

// Get the next highest patient number
int nextPatientNumber(const struct Patient patient[], int max)
{
    int i = 0, nextNum = -1;

    for (i = 0; i < max; i++)
    {
        if (patient[i].patientNumber > max)
        {
            nextNum = patient[i].patientNumber + 1;
        }
    }
    return nextNum;
}

// Find the patient array index by patient number (returns -1 if not found)
int findPatientIndexByPatientNum(int patientNumber,
    const struct Patient patient[], int max)
{
    int i, resurt = -1;

    for (i = 0; i < max; i++)
    {
        if (patient[i].patientNumber == patientNumber)
        {
            resurt = i;
        }
    }

    return resurt;
}

// Sort the order of the data
void sortOrderByDate(struct Appointment* appoint, int max)
{
    int i = 0, j = 0;
    //struct Appointment temp = {{0}};

    for (i = max - 1; i > 0; i--)
    {
        for (j = 0; j < i; j++)
        {
            if (appoint[j].date.year > appoint[j + 1].date.year)
            {
                changeOrder(appoint, j, j + 1);
            }
            else if (appoint[j].date.year == appoint[j + 1].date.year)
            {
                if (appoint[j].date.month > appoint[j + 1].date.month)
                {
                    changeOrder(appoint, j, j + 1);
                }
                else if (appoint[j].date.month == appoint[j + 1].date.month)
                {
                    if (appoint[j].date.day > appoint[j + 1].date.day)
                    {
                        changeOrder(appoint, j, j + 1);
                    }
                    else if (appoint[j].date.day == appoint[j + 1].date.day)
                    {
                        if (appoint[j].time.hour > appoint[j + 1].time.hour)
                        {
                            changeOrder(appoint, j, j + 1);
                        }
                        else if (appoint[j].time.hour == appoint[j + 1].time.hour)
                        {
                            if (appoint[j].time.min > appoint[j + 1].time.min)
                            {
                                changeOrder(appoint, j, j + 1);
                            }
                        }
                    }
                }
            }
        }
    }
}

void changeOrder(struct Appointment* appoint, int i, int j)
{
    struct Appointment temp;
    temp = appoint[i];
    appoint[i] = appoint[j];
    appoint[j] = temp;
}

int leafYear(int year)
{
    int output = 0;

    if (year % 4 == 0)
    {
        output = 1;

        if (year % 100 == 0)
        {
            output = 0;

            if (year % 400 == 0)
            {
                output = 1;
            }
        }
    }
    return output;
}

// Get user input for a new patient record
void inputPatient(struct Patient* patient)
{
    printf("Patient Data Input\n");
    printf("------------------\n");
    printf("Number: %05d\n", (*patient).patientNumber);
    printf("Name  : ");
    inputCString((*patient).name, 0, NAME_LEN);

    printf("\n");

    inputPhoneData(&patient->phone);
}

// Get user input for phone contact information
void inputPhoneData(struct Phone* phone)
{
    int selection;

    printf("Phone Information\n");
    printf("-----------------\n");
    printf("How will the patient like to be contacted?\n");
    printf("1. Cell\n");
    printf("2. Home\n");
    printf("3. Work\n");
    printf("4. TBD\n");
    printf("Selection: ");

    selection = inputIntRange(1, 4);
    switch (selection)
    {
    case 1:
        strcpy((*phone).description, "CELL");
        break;
    case 2:
        strcpy((*phone).description, "HOME");
        break;
    case 3:
        strcpy((*phone).description, "WORK");
        break;
    case 4:
        strcpy((*phone).description, "TBD");
        break;
    }

    if (selection != 4)
    {
        printf("\n");
        printf("Contact: %s\n", (*phone).description);
        printf("Number : ");
        inputCString(phone->number, PHONE_LEN, PHONE_LEN);
    }
    else
    {
        *phone->number = 0000000000;
    }
}

// Import patient data from file into a Patient array (returns # of records read)
int importPatients(const char* datafile, struct Patient patients[], int max)
{
    FILE *fp = NULL;
    fp = fopen(datafile, "r"); // data file open

    int i = 0;
    while (fscanf(fp, "%d|%15[^|]|%[^|]|%[^\n]", &patients[i].patientNumber, patients[i].name,
        patients[i].phone.description, patients[i].phone.number) >= 3 && i < max - 1)
    {
        i++;
    }
    fclose(fp);

    return i + 1;
}

// Import appointment data from file into an Appointment array (returns # of records read)
int importAppointments(const char* datafile, struct Appointment appoints[], int max)
{
    FILE* fp = NULL;
    fp = fopen(datafile, "r");

    int i = 0;
    while(fscanf(fp, "%d,%d,%d,%d,%d,%d", &appoints[i].patientNum, &appoints[i].date.year,
        &appoints[i].date.month, &appoints[i].date.day, &appoints[i].time.hour, &appoints[i].time.min) == 6 && i < max - 1)
    {
        i++;
    }

    fclose(fp);

    return i;
}