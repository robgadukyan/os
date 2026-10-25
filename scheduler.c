#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PROCESSES 100

typedef struct {
    int pid;
    int arrival_time;
    int burst_time;
    int turnaround_time;
    int waiting_time;
    int response_time;
} Process;

/* One Gantt chart segment; pid == 0 means CPU idle. */
typedef struct {
    int pid;
    int start;
    int end;
} Slot;

static void print_gantt(const Slot *slots, int count)
{
    int i, j;

    printf("Gantt Chart: |");
    for (i = 0; i < count; i++) {
        int width = (slots[i].end - slots[i].start) * 2;
        char label[16];
        int len, left, right;

        if (slots[i].pid == 0)
            strcpy(label, "idle");
        else
            sprintf(label, "P%d", slots[i].pid);
        len = (int)strlen(label);
        if (width < len + 2)
            width = len + 2;
        left = (width - len) / 2;
        right = width - len - left;
        for (j = 0; j < left; j++) putchar(' ');
        printf("%s", label);
        for (j = 0; j < right; j++) putchar(' ');
        putchar('|');
    }
    printf("\nTimeline:     %d", count ? slots[0].start : 0);
    for (i = 0; i < count; i++)
        printf(" -> %d", slots[i].end);
    printf("\n");
}

static void print_results(const char *title, const Slot *slots, int slot_count,
                          const Process *order, int n)
{
    double wt = 0, tat = 0, rt = 0;
    int i;

    printf("\n=== %s ===\n", title);
    print_gantt(slots, slot_count);
    printf("%-7s%-7s%-7s%-7s%-7s%-7s\n", "PID", "AT", "BT", "WT", "TAT", "RT");
    for (i = 0; i < n; i++) {
        printf("%-7d%-7d%-7d%-7d%-7d%-7d\n", order[i].pid, order[i].arrival_time,
               order[i].burst_time, order[i].waiting_time,
               order[i].turnaround_time, order[i].response_time);
        wt += order[i].waiting_time;
        tat += order[i].turnaround_time;
        rt += order[i].response_time;
    }
    printf("\nAverage Waiting Time: %.2f\n", wt / n);
    printf("Average Turnaround Time: %.2f\n", tat / n);
    printf("Average Response Time: %.2f\n", rt / n);
}

/* Runs process p at time *clock, updates metrics and Gantt slots. */
static void run_process(Process *p, int *clock, Slot *slots, int *slot_count)
{
    if (*clock < p->arrival_time) {
        slots[*slot_count].pid = 0;
        slots[*slot_count].start = *clock;
        slots[*slot_count].end = p->arrival_time;
        (*slot_count)++;
        *clock = p->arrival_time;
    }
    p->response_time = *clock - p->arrival_time;
    p->waiting_time = p->response_time; /* non-preemptive */
    slots[*slot_count].pid = p->pid;
    slots[*slot_count].start = *clock;
    *clock += p->burst_time;
    slots[*slot_count].end = *clock;
    (*slot_count)++;
    p->turnaround_time = *clock - p->arrival_time;
}

static int cmp_arrival(const void *a, const void *b)
{
    const Process *x = a, *y = b;
    if (x->arrival_time != y->arrival_time)
        return x->arrival_time - y->arrival_time;
    return x->pid - y->pid;
}

void fcfs(const Process *input, int n)
{
    Process procs[MAX_PROCESSES];
    Slot slots[2 * MAX_PROCESSES];
    int slot_count = 0, clock = 0, i;

    memcpy(procs, input, n * sizeof(Process));
    qsort(procs, n, sizeof(Process), cmp_arrival);
    for (i = 0; i < n; i++)
        run_process(&procs[i], &clock, slots, &slot_count);
    print_results("First Come First Served (FCFS)", slots, slot_count, procs, n);
}

void sjf(const Process *input, int n)
{
    Process procs[MAX_PROCESSES], order[MAX_PROCESSES];
    int done[MAX_PROCESSES] = {0};
    Slot slots[2 * MAX_PROCESSES];
    int slot_count = 0, clock = 0, completed, i;

    memcpy(procs, input, n * sizeof(Process));
    for (completed = 0; completed < n; completed++) {
        int best = -1;

        for (i = 0; i < n; i++) {
            if (done[i]) continue;
            if (procs[i].arrival_time <= clock) {
                if (best < 0 ||
                    procs[i].burst_time < procs[best].burst_time ||
                    (procs[i].burst_time == procs[best].burst_time &&
                     (procs[i].arrival_time < procs[best].arrival_time ||
                      (procs[i].arrival_time == procs[best].arrival_time &&
                       procs[i].pid < procs[best].pid))))
                    best = i;
            }
        }
        if (best < 0) { /* CPU idle: jump to earliest arrival */
            for (i = 0; i < n; i++)
                if (!done[i] && (best < 0 ||
                    procs[i].arrival_time < procs[best].arrival_time ||
                    (procs[i].arrival_time == procs[best].arrival_time &&
                     procs[i].burst_time < procs[best].burst_time)))
                    best = i;
        }
        run_process(&procs[best], &clock, slots, &slot_count);
        done[best] = 1;
        order[completed] = procs[best];
    }
    print_results("Shortest Job First (SJF)", slots, slot_count, order, n);
}

int main(void)
{
    Process procs[MAX_PROCESSES];
    int n, i;

    printf("Enter the number of processes: ");
    if (scanf("%d", &n) != 1 || n < 1 || n > MAX_PROCESSES) {
        fprintf(stderr, "Invalid number of processes (1-%d).\n", MAX_PROCESSES);
        return 1;
    }
    for (i = 0; i < n; i++) {
        printf("Enter the arrival time and burst time for process %d: ", i + 1);
        if (scanf("%d %d", &procs[i].arrival_time, &procs[i].burst_time) != 2 ||
            procs[i].arrival_time < 0 || procs[i].burst_time <= 0) {
            fprintf(stderr, "Invalid input: arrival >= 0 and burst > 0 required.\n");
            return 1;
        }
        procs[i].pid = i + 1;
        procs[i].turnaround_time = procs[i].waiting_time = procs[i].response_time = 0;
    }

    fcfs(procs, n);
    sjf(procs, n);
    return 0;
}
