#include <gtk/gtk.h>

// �ݹ� �Լ�: ��ư Ŭ�� �� ȣ��
static void on_calculate_clicked(GtkWidget *widget, gpointer data) {
    GtkEntry **entries = (GtkEntry **)data; // ���޵� ���� �迭
    const char *num1_text = gtk_entry_get_text(entries[0]);
    const char *num2_text = gtk_entry_get_text(entries[1]);
    const char *operator = gtk_button_get_label(GTK_BUTTON(widget));

    double num1 = atof(num1_text);
    double num2 = atof(num2_text);
    double result = 0.0;

    if (g_strcmp0(operator, "+") == 0) {
        result = num1 + num2;
    } else if (g_strcmp0(operator, "-") == 0) {
        result = num1 - num2;
    } else if (g_strcmp0(operator, "*") == 0) {
        result = num1 * num2;
    } else if (g_strcmp0(operator, "/") == 0) {
        if (num2 != 0) {
            result = num1 / num2;
        } else {
            gtk_entry_set_text(entries[2], "Error: Divide by zero");
            return;
        }
    }

    // ����� ��� �Է�â�� ǥ��
    char result_text[64];
    snprintf(result_text, sizeof(result_text), "%.2f", result);
    gtk_entry_set_text(entries[2], result_text);
}

int main(int argc, char *argv[]) {
    GtkWidget *window, *grid, *entry1, *entry2, *entry_result;
    GtkWidget *button_add, *button_sub, *button_mul, *button_div;
    GtkEntry *entries[3];

    gtk_init(&argc, &argv);

    // ���� â ����
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Simple Calculator");
    gtk_window_set_default_size(GTK_WINDOW(window), 300, 200);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // �׸��� ���̾ƿ� ����
    grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(window), grid);

    // �Է�â ����
    entry1 = gtk_entry_new();
    entry2 = gtk_entry_new();
    entry_result = gtk_entry_new();

    gtk_grid_attach(GTK_GRID(grid), entry1, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), entry2, 1, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), entry_result, 2, 0, 1, 1);

    // ��� ��ư ����
    button_add = gtk_button_new_with_label("+");
    button_sub = gtk_button_new_with_label("-");
    button_mul = gtk_button_new_with_label("*");
    button_div = gtk_button_new_with_label("/");

    gtk_grid_attach(GTK_GRID(grid), button_add, 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), button_sub, 1, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), button_mul, 0, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), button_div, 1, 2, 1, 1);

    // �ݹ鿡 ������ �Է�â �迭
    entries[0] = GTK_ENTRY(entry1);
    entries[1] = GTK_ENTRY(entry2);
    entries[2] = GTK_ENTRY(entry_result);

    // ��ư Ŭ�� �̺�Ʈ ����
    g_signal_connect(button_add, "clicked", G_CALLBACK(on_calculate_clicked), entries);
    g_signal_connect(button_sub, "clicked", G_CALLBACK(on_calculate_clicked), entries);
    g_signal_connect(button_mul, "clicked", G_CALLBACK(on_calculate_clicked), entries);
    g_signal_connect(button_div, "clicked", G_CALLBACK(on_calculate_clicked), entries);

    // â ǥ��
    gtk_widget_show_all(window);

    gtk_main();

    return 0;
}
