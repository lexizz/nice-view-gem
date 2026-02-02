#include <stdlib.h>
#include <zephyr/kernel.h>
#include "animation.h"

#include <zmk/event_manager.h>
#include <zmk/events/activity_state_changed.h>

#include <zephyr/logging/log.h>
LOG_MODULE_DECLARE(zmk, CONFIG_ZMK_LOG_LEVEL);

LV_IMG_DECLARE(crystal_01);
LV_IMG_DECLARE(crystal_02);
LV_IMG_DECLARE(crystal_03);
LV_IMG_DECLARE(crystal_04);
LV_IMG_DECLARE(crystal_05);
LV_IMG_DECLARE(crystal_06);
LV_IMG_DECLARE(crystal_07);
LV_IMG_DECLARE(crystal_08);
LV_IMG_DECLARE(crystal_09);
LV_IMG_DECLARE(crystal_10);
LV_IMG_DECLARE(crystal_11);
LV_IMG_DECLARE(crystal_12);
LV_IMG_DECLARE(crystal_13);
LV_IMG_DECLARE(crystal_14);
LV_IMG_DECLARE(crystal_15);
LV_IMG_DECLARE(crystal_16);

const lv_img_dsc_t *anim_imgs[] = {
    &crystal_01, &crystal_02, &crystal_03, &crystal_04, &crystal_05, &crystal_06,
    &crystal_07, &crystal_08, &crystal_09, &crystal_10, &crystal_11, &crystal_12,
    &crystal_13, &crystal_14, &crystal_15, &crystal_16,
};

#if IS_ENABLED(CONFIG_NICE_VIEW_GEM_ANIMATION)
// Store animation object reference for idle/sleep control
static lv_obj_t *anim_obj = NULL;

// Activity state listener to pause/resume animation
static int animation_activity_listener(const zmk_event_t *eh) {
    struct zmk_activity_state_changed *ev = as_zmk_activity_state_changed(eh);
    if (ev == NULL || anim_obj == NULL) {
        return ZMK_EV_EVENT_BUBBLE;
    }

    if (ev->state == ZMK_ACTIVITY_IDLE || ev->state == ZMK_ACTIVITY_SLEEP) {
        // Stop animation in idle/sleep to save power
        lv_animimg_stop(anim_obj);
        LOG_DBG("Animation stopped (idle/sleep)");
    } else if (ev->state == ZMK_ACTIVITY_ACTIVE) {
        // Resume animation when active
        lv_animimg_start(anim_obj);
        LOG_DBG("Animation started (active)");
    }

    return ZMK_EV_EVENT_BUBBLE;
}

ZMK_LISTENER(animation_activity, animation_activity_listener);
ZMK_SUBSCRIPTION(animation_activity, zmk_activity_state_changed);
#endif

void draw_animation(lv_obj_t *canvas) {
#if IS_ENABLED(CONFIG_NICE_VIEW_GEM_ANIMATION)
    anim_obj = lv_animimg_create(canvas);
    lv_obj_center(anim_obj);

    lv_animimg_set_src(anim_obj, (const void **)anim_imgs, 16);
    lv_animimg_set_duration(anim_obj, CONFIG_NICE_VIEW_GEM_ANIMATION_MS);
    lv_animimg_set_repeat_count(anim_obj, LV_ANIM_REPEAT_INFINITE);
    lv_animimg_start(anim_obj);

    lv_obj_align(anim_obj, LV_ALIGN_TOP_LEFT, 36, 0);
#else
    lv_obj_t *art = lv_img_create(canvas);

    int length = sizeof(anim_imgs) / sizeof(anim_imgs[0]);
    srand(k_uptime_get_32());
    int random_index = rand() % length;
    int configured_index = (CONFIG_NICE_VIEW_GEM_ANIMATION_FRAME - 1) % length;
    int anim_imgs_index = CONFIG_NICE_VIEW_GEM_ANIMATION_FRAME > 0 ? configured_index : random_index;

    lv_img_set_src(art, anim_imgs[anim_imgs_index]);
    lv_obj_align(art, LV_ALIGN_TOP_LEFT, 36, 0);
#endif
}
