/* A watch face to show the hours in catalan.


(c) 2015, Juan Vera

*/

#include <pebble.h>

// windows and layers
static Window *s_main_window;
static TextLayer *s_time_layer;
static TextLayer *s_date_layer;
static BitmapLayer *s_senyera_layer;

// resources
static GFont s_time_font;
static GFont s_time_small_font;
static GBitmap *s_image_senyera;

// options
// TODO: make these configurable
static char align_vertically = 1;
static char align_horizontally = 1;
static char first_ucase = 1;
static char date_layer_size = 20;
static char show_date = 1;
static char show_senyera = 1;
static char invert_colors = 0;

static const char *string_hours[] = {
    "les dotze",
    "la una",
    "les dues",
    "les tres",
    "les quatre",
    "les cinc",
    "les sis",
    "les set",
    "les vuit",
    "les nou",
    "les deu",
    "les onze"
};
// notice this array shows the next hour
static const char *string_hours2[] = {
    "d'una",
    "de dues",
    "de tres",
    "de quatre",
    "de cinc",
    "de sis",
    "de set",
    "de vuit",
    "de nou",
    "de deu",
    "d'onze",
    "de dotze"
};
static const char *string_minutes[] = {
  "",
  " tocades",
  "mig quart ",
  "vora un quart ",
  "un quart ",
  "un quart i mig ",
  "vora dos quarts ",
  "quarts ",
  "dos quarts tocats ",
  "dos quarts i mig ",
  "vora tres quarts ",
  "tres quarts ",
  "tres quarts tocats ",
  "tres quarts i mig ",
  "gairebé "
};
static const char *string_weekdays[] = {
  "diumenge, ",
  "dilluns, ",
  "dimarts, ",
  "dimecres, ",
  "dijous, ",
  "divendres, ",
  "dissabte, ",
};
static const char *string_months[] = {
  " de gener",
  " de febrer",
  " de març",
  " d'abril",
  " de maig",
  " de juny",
  " de juliol",
  " de agost",
  " de setembre",
  " d'octubre",
  " de novembre",
  " de desembre"
};

static void update_time() {
  // Get a tm structure
  time_t temp = time(NULL);
  struct tm *tick_time = localtime(&temp);

  // Create a long-lived buffer
  static char buffer[] = "tres quarts tocats de quatreX";
  static char buffer_date[] = "divendes, 31 de setembreX";
  char buffer_day[]="XXX";
  
  // build the text describing the current time
  int h = tick_time->tm_hour % 12;
  int m = tick_time->tm_min / 4;
  buffer[0] = 0;
  if ( m < 2 ) {
    // la una tocada/les dues tocades
    strcat(buffer, string_hours[h]);
    if ( m == 1 && h == 1 ) {
      strcat(buffer, " tocada");
    } else {
      strcat(buffer, string_minutes[m]);
    }
  } else if ( m == 14 ) {
    // gairebe la una
    strcat(buffer, string_minutes[m]);
    strcat(buffer, string_hours[( h + 1 ) % 12]);
  } else {
    // resta de casos
    strcat(buffer, string_minutes[m]);
    strcat(buffer, string_hours2[h]);
  }
  // This is an ugly but easy fix for an inconvenience:
  // h==4 and (m==8 or m==12) are too big to fit on the screen
  // let's use shorter text in these cases!
  // we could use a shorter font, but I prefer not wasting resources
  if ( h == 3 ) {
    if ( m == 8 ) {
      buffer[0] = 0;
      strcat(buffer, "quarts tocats de quatre");
    } else if ( m == 12 ) {
      buffer[0] = 0;
      strcat(buffer, "3/4 tocats de quatre");
    }
  }
  // change the first letter to ucase (only if it is a letter and not a number!)
  if ( first_ucase == 1 && buffer[0] > 'a' ) {
    buffer[0] = buffer[0] + ( 'A' - 'a' );
  }
  
  // set the text on screen
  text_layer_set_text(s_time_layer, buffer);
  // align vertically, if set
  if ( align_vertically == 1 ) {
    GSize s = text_layer_get_content_size(s_time_layer);
    int y = (164 - date_layer_size - s.h) / 2;
    layer_set_frame(text_layer_get_layer(s_time_layer), GRect(0, y, 144, 144));
  }
  
  // if the date is selected, build the text
  if ( show_date == 1 ) {
    buffer_date[0] = 0;
    strcat(buffer_date, string_weekdays[tick_time->tm_wday]);
    //snprintf(buffer_date, 3, "%d", tick_time->tm_wday);
    snprintf(buffer_day, 3, "%d", tick_time->tm_mday);
    strcat(buffer_date, buffer_day);
    strcat(buffer_date, string_months[tick_time->tm_mon]);
    if ( first_ucase == 1 ) {
      buffer_date[0] = buffer_date[0] + ( 'A' - 'a' );
    }
    
    text_layer_set_text(s_date_layer, buffer_date);
  }    
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  update_time();
}

static void main_window_load(Window *w) {
  // Create resources
  s_time_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_TIME_32));
  s_time_small_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_TIME_12));
  
  // back color
  if ( invert_colors == 1 ){
    window_set_background_color(w, GColorBlack);
  } else {
    window_set_background_color(w, GColorWhite);
  }
  
  // create and configure time text layer
  s_time_layer = text_layer_create(GRect(0, 0, 144, 140));
  text_layer_set_background_color(s_time_layer, GColorClear);
  if ( invert_colors == 1 ) {
    text_layer_set_text_color(s_time_layer, GColorWhite);  
  } else {
    text_layer_set_text_color(s_time_layer, GColorBlack);
  }
  text_layer_set_text(s_time_layer, "prova");
  text_layer_set_font(s_time_layer, s_time_font);
  if ( align_horizontally == 1 ) {
    text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);
  } else {
    text_layer_set_text_alignment(s_time_layer, GTextAlignmentLeft);
  }
  text_layer_set_overflow_mode(s_time_layer, GTextOverflowModeWordWrap);
  layer_add_child(window_get_root_layer(w), text_layer_get_layer(s_time_layer));
  
  // create and configure date text layer
  if ( show_date == 1 ) {
    s_date_layer = text_layer_create(GRect(0, 150, 144, 20));
    text_layer_set_background_color(s_date_layer, GColorClear);
    if ( invert_colors == 1 ) {
      text_layer_set_text_color(s_date_layer, GColorWhite);  
    } else {
      text_layer_set_text_color(s_date_layer, GColorBlack);
    }  
    text_layer_set_text(s_date_layer, "prova");
    text_layer_set_font(s_date_layer, s_time_small_font);
    if ( align_horizontally == 1 ) {
      text_layer_set_text_alignment(s_date_layer, GTextAlignmentCenter);
    } else {
      text_layer_set_text_alignment(s_date_layer, GTextAlignmentLeft);
    }
    text_layer_set_overflow_mode(s_date_layer, GTextOverflowModeWordWrap);
    date_layer_size = 20;    
    layer_add_child(window_get_root_layer(w), text_layer_get_layer(s_date_layer));
  } else {
    date_layer_size = 0;
  }
  
  if ( show_senyera == 1 ) {
    s_image_senyera = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_SENYERA);
    s_senyera_layer = bitmap_layer_create(GRect(0, 164 - date_layer_size, 144, 4));
    bitmap_layer_set_bitmap(s_senyera_layer, s_image_senyera);
    layer_add_child(window_get_root_layer(w), bitmap_layer_get_layer(s_senyera_layer));
  }
}

static void main_window_unload(Window *w) {
  // destroy layers
  text_layer_destroy(s_time_layer);
  if ( show_date == 1 ) {
    text_layer_destroy(s_date_layer);
  }
  if ( show_senyera == 1 ) {
    bitmap_layer_destroy(s_senyera_layer);
    gbitmap_destroy(s_image_senyera);
  }
  
  // destroy resources
  fonts_unload_custom_font(s_time_font);
  fonts_unload_custom_font(s_time_small_font);
}

static void init() {
  // create the window and register handlers
  s_main_window = window_create();
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload    
  });
  
  // show the window on the watch, with animated==true
  window_stack_push(s_main_window, true);
  
  // register services
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
  
  update_time();
}

static void deinit() {
  window_destroy(s_main_window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();  
}