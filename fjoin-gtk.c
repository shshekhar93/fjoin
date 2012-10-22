#include <gtk/gtk.h>
#include <string.h>
#include "joiner.h"

void joinFunction(char fname[])
{
	join(fname);
}

void splitFunction(char fname[], unsigned long long size)
{
	split(fname, size);
}

void splitting(GtkButton *button, gpointer usr_data)
{
	//GThread *thrd1;
	GtkWidget *fileChooser;
	GtkWidget *sizeWdgt;
	GtkWidget *factorWdgt;
	gchar *fname, *factorTxt;
	const gchar *sizeTxt;
	GList *list;
	unsigned long long ofsize, fctr, sval;
	int i;
	
	/* Retrieving data from the passed GList.
	 */
	list = (GList *)usr_data;
	fileChooser = list->data;
	list = g_list_next(list);
	sizeWdgt = list->data;
	list = g_list_next(list);
	factorWdgt = list->data;
	
	fname = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(fileChooser));
	sizeTxt = gtk_entry_get_text(GTK_ENTRY(sizeWdgt));
	factorTxt = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(factorWdgt));
	
	/* Creating numbers from text
	 */
	sval = 0;
	for(i=0; sizeTxt[i]!='\0'; i++)
	{
		if(sizeTxt[i]<'0' || sizeTxt[i]>'9')
			break;
		sval = (sval * 10) + (sizeTxt[i] - '0');
	}
	if(sizeTxt[i]!='\0')
	{
		g_print("Invalid size");
		
		/* Insert error dialog later.
		 */
		return ;
	}
	if(strcmp(factorTxt, "KB")==0)
	{
		fctr = 1024;
	}
	else if (strcmp(factorTxt, "MB")==0)
	{
		fctr = 1024 * 1024;
	}
	else if (strcmp(factorTxt, "GB")==0)
	{
		fctr = 1024 * 1024 * 1024;
	}
	else
	{
		g_print("Please select a size");
		
		/* Insert error dialog later.
		 */
		return ;
	}
	
	/* Calculating the output file size.
	 */
	ofsize = sval * fctr;
	/*g_print("%s %lld %s %lld %s %lld\n", "sval =", sval, "fctr =", fctr, "ofszie=", ofsize);*/
	
	if(fname != NULL)
	{
		g_print("%s %s %s %lld\n","File selected for splitting is: ", fname, "with size", ofsize);
		splitFunction(fname, ofsize);
	}
	else
		g_print("No file selected.\n");
}

void joining(GtkButton *button, gpointer usr_data)
{
	GtkWidget *fileChooser;
	gchar *fname;
	
	fileChooser = (GtkWidget *)usr_data;
	fname = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(fileChooser));
	
	
	
	if(fname != NULL)
	{
		g_print("%s %s\n","File selected for join is: ", fname);
		joinFunction(fname);
	}
	else
		g_print("No file selected.\n");
}

void splitActive(GtkWidget *box)
{
	GtkWidget *hbox;
	GtkWidget *sizeTxt;
	GtkWidget *factor;
	GtkWidget *splitB, *fileChooser;
	GList *list;
	
	splitB = gtk_button_new_with_label("Split");
	fileChooser = gtk_file_chooser_button_new("Open", GTK_FILE_CHOOSER_ACTION_OPEN);
	sizeTxt = gtk_entry_new();
	factor = gtk_combo_box_text_new();
	
	gtk_entry_set_alignment(GTK_ENTRY(sizeTxt), 1);
	
	gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(factor), "KB");
	gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(factor), "MB");
	gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(factor), "GB");
	
	gtk_widget_set_size_request(splitB, 75, 30);
	gtk_widget_set_size_request(fileChooser, 100, 30);
	gtk_widget_set_size_request(sizeTxt, 100, 30);
	gtk_widget_set_size_request(factor, 75, 30);
	
	hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
	gtk_box_pack_start(GTK_BOX(hbox), gtk_label_new("Select file to split: "), FALSE, TRUE, 4);
	gtk_box_pack_end(GTK_BOX(hbox), fileChooser, FALSE, FALSE, 4);
	gtk_box_pack_start(GTK_BOX(box), hbox, FALSE, FALSE, 20);
	
	hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
	gtk_box_pack_start(GTK_BOX(hbox), gtk_label_new("Enter size: "), FALSE, TRUE, 4);
	gtk_box_pack_end(GTK_BOX(hbox), factor, FALSE, FALSE, 4);
	gtk_box_pack_end(GTK_BOX(hbox), sizeTxt, FALSE, FALSE, 4);
	gtk_box_pack_start(GTK_BOX(box), hbox, FALSE, FALSE, 20);
	
	hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
	gtk_box_pack_start(GTK_BOX(hbox), splitB, TRUE, FALSE, 4);
	gtk_box_pack_start(GTK_BOX(box), hbox, FALSE, FALSE, 20);
	
	/* Creating a list of poiners to be passed to splitting function.
	 */
	list = NULL;
	list = g_list_append(list, fileChooser);
	list = g_list_append(list, sizeTxt);
	list = g_list_append(list, factor);
	
	g_signal_connect(splitB, "clicked", G_CALLBACK(splitting), list);
	
	gtk_widget_show_all(box);
	
	g_print("Split is active\n");
}

void joinActive(GtkWidget *box)
{
	GtkWidget *hbox;
	GtkWidget *splitB, *fileChooser;
	
	splitB = gtk_button_new_with_label("Join");
	fileChooser = gtk_file_chooser_button_new("Open", GTK_FILE_CHOOSER_ACTION_OPEN);
	
	gtk_widget_set_size_request(splitB, 75, 30);
	gtk_widget_set_size_request(fileChooser, 100, 30);
	
	hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
	gtk_box_pack_start(GTK_BOX(hbox), gtk_label_new("Select file to join: "), FALSE, TRUE, 4);
	gtk_box_pack_end(GTK_BOX(hbox), fileChooser, FALSE, FALSE, 4);
	gtk_box_pack_start(GTK_BOX(box), hbox, FALSE, FALSE, 50);
	
	hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
	gtk_box_pack_start(GTK_BOX(hbox), splitB, TRUE, FALSE, 4);
	gtk_box_pack_start(GTK_BOX(box), hbox, FALSE, FALSE, 0);
	
	g_signal_connect(splitB, "clicked", G_CALLBACK(joining), fileChooser);
	
	gtk_widget_show_all(box);
	
	g_print("Join is active\n");
}

void sptoggle(GtkToggleButton *tb, gpointer usr_data)
{
	GtkWidget *vbox;
	GList *children, *it;
	vbox = (GtkWidget *)usr_data;
	
	/* Removing all components from the vbox
	 */
	children = gtk_container_get_children(GTK_CONTAINER(vbox));
	for(it = children; it != NULL; it = g_list_next(it))
	{
		gtk_widget_destroy(GTK_WIDGET(it->data));
	}
	
	if(gtk_toggle_button_get_active(tb))
	{
		splitActive(vbox);
	}
	else
	{
		joinActive(vbox);
	}
}

int main( int argc, char *argv[])
{
	GtkWidget *window;
	GtkWidget *baseVBox;
	GtkWidget *topBar, *bottomBox;
	GtkWidget *spButton, *jnButton;

	if( ! g_thread_supported() )
        g_thread_init( NULL );

    /* Secure gtk */
    gdk_threads_init();

    /* Obtain gtk's global lock */
    gdk_threads_enter();

	gtk_init(&argc, &argv);

	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(window),"fjoin");
	gtk_window_set_default_size(GTK_WINDOW(window), 300, 300);


	baseVBox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
	topBar = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
	bottomBox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
	jnButton = gtk_radio_button_new_with_label_from_widget(NULL, "Join");
	spButton = gtk_radio_button_new_with_label_from_widget(GTK_RADIO_BUTTON(jnButton), "Split");
	
	/* To make the jnButton selected by default, and
	   call the joinActive() function.
	 */
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(jnButton),TRUE);
	joinActive(bottomBox);
	
	gtk_box_pack_start(GTK_BOX(baseVBox), topBar, FALSE, FALSE, 4);
	gtk_box_pack_start(GTK_BOX(baseVBox), gtk_separator_new(GTK_ORIENTATION_HORIZONTAL), FALSE, TRUE, 5);
	gtk_box_pack_start(GTK_BOX(baseVBox), bottomBox, TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(topBar), jnButton, TRUE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(topBar), spButton, TRUE, FALSE, 0);
	
	gtk_container_add(GTK_CONTAINER(window), baseVBox);

	g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
	g_signal_connect(spButton, "toggled", G_CALLBACK(sptoggle), bottomBox);

	gtk_widget_set_size_request(window, 300, 300);
	gtk_widget_show_all(window);
	
	gtk_main();
	
	gdk_threads_leave();

	return 0;
}
