#include <iostream>
#include <gtk/gtk.h>

const char * fiforead = "/tmp/RobBSmap"; // du robot vers l'outil
const char * fifowrite = "/tmp/BSmapRob"; // de l'outil vers le robot
int fd;
void change(GtkWidget *pb,gpointer data);
	GtkWidget *pWindow;
	GtkWidget *pLabel;
	GtkWidget *pTable;
	GtkWidget *pButton[4];

int main(int argc, char **argv)
{
	GdkRGBA c_red,c_blue;//,c_green;
	c_red.alpha =1;c_red.red = 1;c_red.green = 0;c_red.blue=0;
	c_blue.alpha =1;c_blue.red = 0;c_blue.green = 0;c_blue.blue=1;
	//c_green.alpha =1;c_green.red = 0;c_green.green = 1;c_green.blue=0;


	// la fenêtre principale
	//GtkWidget *pQuitBtn;
	gchar *TexteConverti;
	
	gtk_init(&argc, &argv);
	
	// paramètres de la fenêtre
	pWindow = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(pWindow),"Nom de la fenêtre");
	gtk_window_set_default_size(GTK_WINDOW(pWindow),500,700);
	g_signal_connect(G_OBJECT(pWindow),"delete-event",G_CALLBACK(gtk_main_quit),NULL);
	// table
	pTable=gtk_table_new(18,10,FALSE); //homogène
	gtk_container_add(GTK_CONTAINER(pWindow),GTK_WIDGET(pTable));
	
	pButton[0]=gtk_button_new_with_label("Button 1");
	pButton[1]=gtk_button_new_with_label("Button 2");
	pButton[2]=gtk_button_new_with_label("Button 3");
	pButton[3]=gtk_button_new_with_label("Button 4");
	
	// label
	TexteConverti = g_locale_to_utf8("le site\n",-1,NULL,NULL,NULL);
	pLabel=gtk_label_new(TexteConverti);
	g_free(TexteConverti);
	gtk_table_attach_defaults(GTK_TABLE(pTable),pButton[0],0,2,0,1);
	gtk_table_attach_defaults(GTK_TABLE(pTable),pButton[1],0,1,1,2);
	gtk_table_attach_defaults(GTK_TABLE(pTable),pButton[2],1,2,1,2);
	gtk_table_attach_defaults(GTK_TABLE(pTable),pButton[3],0,2,2,3);
	gtk_table_attach_defaults(GTK_TABLE(pTable),pLabel,2,3,2,3);
	gtk_widget_override_background_color(GTK_WIDGET(pLabel),GTK_STATE_FLAG_NORMAL, &c_blue);
	gtk_widget_override_color(GTK_WIDGET(pLabel),GTK_STATE_FLAG_NORMAL, &c_red);
	g_signal_connect(G_OBJECT(pButton[0]),"clicked",G_CALLBACK(change),(GtkWidget *)pLabel);
	g_signal_connect(G_OBJECT(pButton[1]),"clicked",G_CALLBACK(change),(GtkWidget *)pLabel);
	
/*	//gtk_container_add(GTK_CONTAINER(pWindow),pLabel);

	pQuitBtn = gtk_button_new_with_label("Quitter");
	g_signal_connect(G_OBJECT(pQuitBtn),"clicked",G_CALLBACK(gtk_main_quit),NULL);
	gtk_container_add(GTK_CONTAINER(pWindow),pQuitBtn);
*/
	gtk_widget_show_all(pWindow);
	
	
	gtk_main();
	return(0);
//	printf("tout va bien\n");
}
void change(GtkWidget *pb,gpointer data)
{
	if (pb == pButton[0])
		gtk_label_set_text(GTK_LABEL((GtkWidget *)data),(const gchar *)("salut"));
	if (pb == pButton[1])
		gtk_label_set_text(GTK_LABEL((GtkWidget *)data),(const gchar *)("coucou"));

}
