#include <gtk/gtk.h>

// ��ư Ŭ�� �� ȣ��Ǵ� �ݹ� �Լ�
static void on_button_clicked(GtkWidget *widget, gpointer data) {
    g_print("Button clicked: %s\n", (char *)data);
}

int main(int argc, char *argv[]) {
    GtkWidget *window;   // â ����
    GtkWidget *button;   // ��ư ����
    GtkWidget *box;      // ���̾ƿ��� ���� �ڽ� �����̳�

    // GTK �ʱ�ȭ
    gtk_init(&argc, &argv);

    // â ����
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "GTK+ Simple UI");
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 200);
    gtk_container_set_border_width(GTK_CONTAINER(window), 10);

    // â �ݱ� �̺�Ʈ ó��
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // �ڽ� ���̾ƿ� ����
    box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_container_add(GTK_CONTAINER(window), box);

    // ��ư ����
    button = gtk_button_new_with_label("Click Me!");
    g_signal_connect(button, "clicked", G_CALLBACK(on_button_clicked), "Button 1");
    gtk_box_pack_start(GTK_BOX(box), button, TRUE, TRUE, 0);

    // �߰� ��ư
    GtkWidget *button2 = gtk_button_new_with_label("Another Button");
    g_signal_connect(button2, "clicked", G_CALLBACK(on_button_clicked), "Button 2");
    gtk_box_pack_start(GTK_BOX(box), button2, TRUE, TRUE, 0);

    // â ǥ��
    gtk_widget_show_all(window);

    // GTK ���� ���� ����
    gtk_main();

    return 0;
}
