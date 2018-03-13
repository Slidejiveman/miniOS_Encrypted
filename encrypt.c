#include <stdlib.h>
#include <stdio.h>
#include <time.h>

/* prototypes  */
int encrypt();
int generate_key();


/* main */
int main() 
{
    // generate one-time pad key
    srand(time(NULL));
    FILE *kfp;
    kfp = fopen("key", "w+");
    int key = generate_key(0);
    fprintf(kfp, "%d\n", key);
    printf("key generated\n");
    fclose(kfp);

    // use the key to encrypt files
    encrypt(key);

    return EXIT_SUCCESS;
}

int generate_key(int val)
{
    int retval = rand();
    return retval;
}

int encrypt(int key)
{
    int i;
    static char cfgPm[100][10];
    static int encrypted_inputs[200];
    FILE *fprocon, *fcmgr, *fmmgr;

    // Producer-Consumer
    fprocon = fopen ("procon", "r+");
    for (i = 0; fscanf(fprocon, "%s\n", cfgPm[i]) != EOF; i++)
    {
        printf("p%d=[%s]\n", i, cfgPm[i]);
    }
    cfgPm[i][0] = '\0';
    
    fseek(fprocon, 0, SEEK_SET);

    for(i = 0; cfgPm[i][0]; ++i)
    {
        encrypted_inputs[i] = atoi(cfgPm[i]) ^ key ;
        fprintf(fprocon, "%d\n", encrypted_inputs[i]);
    }    
    fclose(fprocon);
    
    // Scheduler
    fcmgr = fopen ("cmgr", "r+");
    for (i = 0; fscanf(fcmgr, "%s\n", cfgPm[i]) != EOF; i++)
    {
        printf("p%d=[%s]\n", i, cfgPm[i]);
    }
    cfgPm[i][0] = '\0';

    fseek(fcmgr, 0, SEEK_SET);

    for(i = 0; cfgPm[i][0]; ++i)
    {
        encrypted_inputs[i] = atoi(cfgPm[i]) ^ key ;
        fprintf(fcmgr, "%d\n", encrypted_inputs[i]);
    }
    fclose(fcmgr);

    // Memory Manager
    fmmgr = fopen ("mmgr", "r+");
    for (i = 0; fscanf(fmmgr, "%s\n", cfgPm[i]) != EOF; i++)
    {
        printf("p%d=[%s]\n", i, cfgPm[i]);
    }
    cfgPm[i][0] = '\0';

    fseek(fmmgr, 0, SEEK_SET);

    for(i = 0; cfgPm[i][0]; ++i)
    {
        encrypted_inputs[i] = atoi(cfgPm[i]) ^ key ;
        fprintf(fmmgr, "%d\n", encrypted_inputs[i]);
    }
    fclose(fmmgr);

    return EXIT_SUCCESS;
}

