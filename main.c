#include <stdlib.h>
#include <gtk/gtk.h>
#include <math.h>

int checkStartButton = 0;
int isNeedGenerate = 0;
int genNum = 1;
char windowTitle[100];
struct coord_t {
    int x;
    int y;
} size;
typedef struct coord_t coord_t;

void generate(int **field);
int checkCell(int **field, int i, int j);

void startButtonClick(GtkWidget *startButton, GtkWidget *window);
void stepbystepButtonClick(GtkWidget *stepbystepButton, GtkWidget *window);
static gboolean fieldClick(GtkWidget *window, GdkEventButton *event, int **field);
static gboolean on_draw_event(GtkWidget *darea, cairo_t *cairoDrawPlace, int **field);
void do_drawing(cairo_t *cairoDrawPlace, GtkWidget *darea, int **field);
static gboolean time_handler(GtkWidget *window);
void closeButton(GtkWidget *window, int **field);

int main(int argc, char **argv)
{
    GtkWidget *window;
    GtkWidget *frame;
    GtkWidget *startButton;
    GtkWidget *stepbystepButton;
    GtkWidget *darea;
    int **field, i;

    size.x = 20;
    size.y = 23;
    field = (int **) calloc(size.x, sizeof(int *));
    for (i = 0; i < size.x; i++) {
        field[i] = (int *) calloc(size.y, sizeof(int));
    }

    gtk_init(&argc, &argv);

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_resizable(GTK_WINDOW(window), FALSE);
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    gtk_window_resize(GTK_WINDOW(window), 485, 510);  /*need to fix*/
    gtk_window_set_title(GTK_WINDOW(window), "Game of life");


    frame = gtk_fixed_new();
    gtk_container_add(GTK_CONTAINER(window), frame);
    gtk_container_set_border_width(GTK_CONTAINER(frame), 10);

    darea = gtk_drawing_area_new();
    gtk_fixed_put(GTK_FIXED(frame), darea, 0, 0);
    gtk_widget_set_size_request(darea, 465, 410);

    startButton = gtk_button_new_with_label("Start");
    gtk_widget_set_size_request(startButton, 100, 50);
    gtk_fixed_put(GTK_FIXED(frame), startButton, 120, 420);

    stepbystepButton = gtk_button_new_with_label("Step-by-step");
    gtk_widget_set_size_request(stepbystepButton, 100, 50);
    gtk_fixed_put(GTK_FIXED(frame), stepbystepButton, 240, 420);

    gtk_widget_add_events(window, GDK_BUTTON_PRESS_MASK);
    g_signal_connect(window, "button-press-event", G_CALLBACK(fieldClick), field);
    g_signal_connect(window, "destroy", G_CALLBACK(closeButton), field);
    g_signal_connect(startButton, "clicked", G_CALLBACK(startButtonClick), window);
    g_signal_connect(stepbystepButton, "clicked", G_CALLBACK(stepbystepButtonClick), window);
    g_signal_connect(G_OBJECT(darea), "draw", G_CALLBACK(on_draw_event), field);
    gtk_widget_show_all(window);
    gtk_main();

    return 0;
}

int fInputNumericalData(FILE *lifeInput)
{
    int number;
    char wrongSymbol;
    if (!fscanf(lifeInput, "%d", &number)) {
        wrongSymbol = getc(lifeInput);
        printf("'%c' is not a number.\n", wrongSymbol);
        exit(1);
    }
    return number;
}

void generate(int **field)
{
    int i, j;
    int bufferField[size.x][size.y];
    for (i = 0; i < size.x; i++) {
        for (j = 0; j < size.y; j++) {
            bufferField[i][j] = checkCell(field, i, j);
        }
    }
    for (i = 0; i < size.x; i++) {
        for (j = 0; j < size.y; j++) {
            field[i][j] = bufferField[i][j];
        }
    }
}

int checkCell(int **field, int i, int j)
{
    int up, down, right, left, sum;
    (j == 0) ? (up = size.y - 1) : (up = j - 1);
    (i == 0) ? (left = size.x - 1) : (left = i - 1);
    (j == size.y - 1) ? (down = 0) : (down = j + 1);
    (i == size.x - 1) ? (right = 0) : (right = i + 1);
    sum = field[i][up] + field[i][down] + field[left][j] + field[right][j]
        + field[left][up] + field[right][up] + field[left][down] + field[right][down];
    if (sum == 3) {
        return 1;
    } else if (field[i][j] && sum == 2) {
        return 1;
    }
    return 0;
}


void startButtonClick(GtkWidget *startButton, GtkWidget *window)
{
    if (checkStartButton) {
        checkStartButton = 0;
        gtk_button_set_label(GTK_BUTTON(startButton), "Start");
    } else {
        checkStartButton = 1;
        gtk_button_set_label(GTK_BUTTON(startButton), "Stop ");
    }
    g_timeout_add(100, (GSourceFunc) time_handler, window);
}

void stepbystepButtonClick(GtkWidget *stepbystepButton, GtkWidget * window)
{
    isNeedGenerate = 1;
    gtk_widget_queue_draw(window);
    gtk_window_set_title(GTK_WINDOW(window), windowTitle);
}

static gboolean time_handler(GtkWidget *window)
{
    if (!checkStartButton) {
        return FALSE;
    }
    isNeedGenerate = 1;
    gtk_widget_queue_draw(window);
    gtk_window_set_title(GTK_WINDOW(window), windowTitle);
    return TRUE;
}

static gboolean fieldClick(GtkWidget *window, GdkEventButton *event, int **field)
{
    int i, j;
    if (event->button == 1) {
        j = event->x - ((int) (event->x - 10) % 20);
        i = event->y - ((int) (event->y - 10) % 20);
        if ((i / 20) >= size.x || (j / 20) >= size.y) {
            return TRUE;
        }
        if (field[i / 20][j / 20]) {
            field[i / 20][j / 20] = 0;
        } else {
            field[i / 20][j / 20] = 1;
        }
        isNeedGenerate = 0;
        gtk_widget_queue_draw(window);
        return TRUE;
    }
    if (event->button == 3) {
        isNeedGenerate = 1;
        gtk_widget_queue_draw(window);
        gtk_window_set_title(GTK_WINDOW(window), windowTitle);
        g_timeout_add(100, (GSourceFunc) time_handler, window);
    }
    return TRUE;
}

static gboolean on_draw_event(GtkWidget *darea, cairo_t *cairoDrawPlace, int **field)
{
    cairoDrawPlace = gdk_cairo_create(gtk_widget_get_window(darea));
    if (isNeedGenerate) {
        generate(field);
        genNum++;
        sprintf(windowTitle, "Game of life || Generation number: %d", genNum);
    }
    do_drawing(cairoDrawPlace, darea, field);
    cairo_destroy(cairoDrawPlace);
    return FALSE;
}

void do_drawing(cairo_t *cairoDrawPlace, GtkWidget *darea, int **field)
{
    int i, j;
    cairo_set_source_rgb(cairoDrawPlace, 0.761, 0.796, 0.251);
    cairo_paint(cairoDrawPlace);
    cairo_set_source_rgba(cairoDrawPlace, 0.102, 0.094, 0.09, 1);
    cairo_set_line_width(cairoDrawPlace, 2);
    cairo_set_line_cap(cairoDrawPlace, CAIRO_LINE_CAP_ROUND);
    cairo_translate(cairoDrawPlace, 6, 6);

    for (i = 0; i < size.x; i++) {
        for (j = 0; j < size.y; j++) {
            if (field[i][j]) {
                cairo_rectangle(cairoDrawPlace, j * 20, i * 20, 12, 12);
            }
        }
    }
    cairo_stroke(cairoDrawPlace);
}

void closeButton(GtkWidget *window, int **field)
{
    int i;
    for (i = 0; i < size.x; i++) {
        free(field[i]);
    }
    free(field);
    gtk_main_quit();
}
