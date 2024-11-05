#include "yyjson.h"
#include "algorithm.h"

void process_value(yyjson_mut_doc *original_mut_doc, yyjson_mut_doc *template_mut_doc, yyjson_mut_val *template_cur_root, yyjson_mut_val *cur_root_parent, size_t cur_template_root_idx, yyjson_mut_val *cur_template_root_key, bool is_array) {
    if (yyjson_mut_is_str(template_cur_root)) {
        const char *template_str = yyjson_mut_get_str(template_cur_root);
        yyjson_ptr_err err;
        yyjson_mut_val *val = yyjson_mut_doc_ptr_get(original_mut_doc, template_str);
        yyjson_mut_val *new_val = yyjson_mut_val_mut_copy(template_mut_doc, val);
        
        if (is_array) {
            yyjson_mut_arr_replace(cur_root_parent, cur_template_root_idx, new_val);
        } else {
            yyjson_mut_obj_replace(cur_root_parent, cur_template_root_key, new_val);
        }
    } else if (yyjson_mut_is_obj(template_cur_root)) {
        size_t idx, max;
        yyjson_mut_val *key, *val;
        yyjson_mut_obj_foreach(template_cur_root, idx, max, key, val) {
            process_value(original_mut_doc, template_mut_doc, val, template_cur_root, idx, key, false);
        }
    } else if (yyjson_mut_is_arr(template_cur_root)) {
        size_t idx, max;
        yyjson_mut_val *val;
        yyjson_mut_arr_foreach(template_cur_root, idx, max, val) {
            process_value(original_mut_doc, template_mut_doc, val, template_cur_root, idx, NULL, true);
        }
    }
}

void start_process(yyjson_mut_doc *original_mut_doc, yyjson_mut_doc *template_mut_doc) {
    yyjson_mut_val *template_mut_root = yyjson_mut_doc_get_root(template_mut_doc);
    
    if (yyjson_mut_is_obj(template_mut_root)) {
        size_t idx, max;
        yyjson_mut_val *key, *val;
        yyjson_mut_obj_foreach(template_mut_root, idx, max, key, val) {
            process_value(original_mut_doc, template_mut_doc, val, template_mut_root, idx, key, false);
        }
    } else if (yyjson_mut_is_arr(template_mut_root)) {
        size_t idx, max;
        yyjson_mut_val *val;
        yyjson_mut_arr_foreach(template_mut_root, idx, max, val) {
            process_value(original_mut_doc, template_mut_doc, val, template_mut_root, idx, NULL, true);
        }
    } else if (yyjson_mut_is_str(template_mut_root)) {
        const char *template_str = yyjson_mut_get_str(template_mut_root);
        yyjson_mut_val *new_val = yyjson_mut_val_mut_copy(template_mut_doc, yyjson_mut_doc_ptr_get(original_mut_doc, template_str));
        yyjson_mut_doc_set_root(template_mut_doc, new_val);
    }
}

char *transform(char *original_str, char *template_str) {
    yyjson_doc *original_doc = yyjson_read(original_str, strlen(original_str), 0);
    yyjson_mut_doc *original_mut_doc = yyjson_doc_mut_copy(original_doc, NULL);
    yyjson_doc *template_doc = yyjson_read(template_str, strlen(template_str), 0);
    yyjson_mut_doc *template_mut_doc = yyjson_doc_mut_copy(template_doc, NULL);
    yyjson_doc_free(original_doc);
    yyjson_doc_free(template_doc);

    start_process(original_mut_doc, template_mut_doc);
    char *result = yyjson_mut_write(template_mut_doc, 0, NULL);
    
    yyjson_mut_doc_free(original_mut_doc);
    yyjson_mut_doc_free(template_mut_doc);
    
    return result;
}
