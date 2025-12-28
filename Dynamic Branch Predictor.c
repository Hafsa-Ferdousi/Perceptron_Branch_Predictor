#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_HISTORY 16
#define MAX_PERCEPTRONS 256
#define THRESHOLD 32

typedef struct {
    int weights[MAX_HISTORY + 1];
} Perceptron;




Perceptron perceptron_table[MAX_PERCEPTRONS];
int branch_history[MAX_HISTORY];
int history_size;
int total_perceptrons;



void initialize_perceptrons(int history_len, int perceptron_count) {
    history_size = history_len;
    total_perceptrons = perceptron_count;

    memset(branch_history, 0, sizeof(branch_history));

    for (int i = 0; i < total_perceptrons; i++) {
        for (int j = 0; j <= history_size; j++) {
            perceptron_table[i].weights[j] = 0;
        }
    }
}


int get_prediction(Perceptron *perceptron, int *history) {
    int sum = perceptron->weights[0];

    for (int i = 0; i < history_size; i++) {
        sum += perceptron->weights[i + 1] * history[i];
    }
    return sum;
}



void train_perceptron(Perceptron *perceptron, int *history, int actual_outcome) {
    int prediction_value = get_prediction(perceptron, history);
    int predicted_outcome = (prediction_value >= 0) ? 1 : -1;

    if (predicted_outcome != actual_outcome || abs(prediction_value) <= THRESHOLD) {
        perceptron->weights[0] += actual_outcome;

        for (int i = 0; i < history_size; i++) {
            perceptron->weights[i + 1] += actual_outcome * history[i];
        }
    }
}


void update_branch_history(int actual_outcome) {

    for (int i = history_size - 1; i > 0; i--) {

        branch_history[i] = branch_history[i - 1];
    }

    branch_history[0] = actual_outcome;
}

int main() {
    int history_len, perceptron_count, branch_count;

    printf("Enter history length: ");
    scanf("%d", &history_len);

    printf("Enter number of perceptrons: ");
    scanf("%d", &perceptron_count);

    initialize_perceptrons(history_len, perceptron_count);

    printf("Enter number of branches to simulate: ");
    scanf("%d", &branch_count);

    for (int i = 0; i < branch_count; i++) {
        int branch_id, actual_outcome;
        printf("\nEnter branch ID (0-%d): ", total_perceptrons - 1);
        scanf("%d", &branch_id);

        printf("Enter actual outcome (1 for taken, -1 for not taken): ");
        scanf("%d", &actual_outcome);

        if (branch_id >= total_perceptrons) {
            printf("Invalid branch ID!\n");
            continue;

        }

        int prediction_value = get_prediction(&perceptron_table[branch_id], branch_history);
        int predicted_outcome = (prediction_value >= 0) ? 1 : -1;

        printf("Prediction: %s\n", predicted_outcome == 1 ? "Taken" : "Not Taken");

        train_perceptron(&perceptron_table[branch_id], branch_history, actual_outcome);
        update_branch_history(actual_outcome);

        printf("Updated history: ");

        for (int j = 0; j < history_size; j++) {
            printf("%d ", branch_history[j]);

        }

        printf("\n");

    }

    return 0;
}
