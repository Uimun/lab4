#include <gtk/gtk.h>

// 버튼 클릭 시 호출되는 콜백 함수
static void on_button_clicked(GtkWidget *widget, gpointer data) {
    g_print("Button clicked: %s\n", (char *)data);
}

int main(int argc, char *argv[]) {
    GtkWidget *window;   // 창 위젯
    GtkWidget *button;   // 버튼 위젯
    GtkWidget *box;      // 레이아웃을 위한 박스 컨테이너

    // GTK 초기화
    gtk_init(&argc, &argv);

    // 창 생성
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "GTK+ Simple UI");
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 200);
    gtk_container_set_border_width(GTK_CONTAINER(window), 10);

    // 창 닫기 이벤트 처리
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // 박스 레이아웃 생성
    box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_container_add(GTK_CONTAINER(window), box);

    // 버튼 생성
    button = gtk_button_new_with_label("Click Me!");
    g_signal_connect(button, "clicked", G_CALLBACK(on_button_clicked), "Button 1");
    gtk_box_pack_start(GTK_BOX(box), button, TRUE, TRUE, 0);

    // 추가 버튼
    GtkWidget *button2 = gtk_button_new_with_label("Another Button");
    g_signal_connect(button2, "clicked", G_CALLBACK(on_button_clicked), "Button 2");
    gtk_box_pack_start(GTK_BOX(box), button2, TRUE, TRUE, 0);

    // 창 표시
    gtk_widget_show_all(window);

    // GTK 메인 루프 실행
    gtk_main();

    return 0;
}
