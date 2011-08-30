.global draw_v_vsu_size
.set draw_v_vsu_size, 0f-draw_v_vsu
.global draw_v_vsu
draw_v_vsu:
	.incbin "draw_v.vsu"
0:

.align 4

.global draw_c_p_psu_size
.set draw_c_p_psu_size, 0f-draw_c_p_psu
.global draw_c_p_psu
draw_c_p_psu:
	.incbin "draw_c_p.psu"
0:

.align 4

.global draw_t_p_psu_size
.set draw_t_p_psu_size, 0f-draw_t_p_psu
.global draw_t_p_psu
draw_t_p_psu:
	.incbin "draw_t_p.psu"
0:

.align 4

.global font_p_psu_size
.set font_p_psu_size, 0f-font_p_psu
.global font_p_psu
font_p_psu:
	.incbin "font_p.psu"
0:

.align 4

.global font_v_vsu_size
.set font_v_vsu_size, 0f-font_v_vsu
.global font_v_vsu
font_v_vsu:
	.incbin "font_v.vsu"
0:

.align 4


.global Verdana_16_png_size
.set Verdana_16_png_size, 0f-Verdana_16_png
.global Verdana_16_png
Verdana_16_png:
	.incbin "Verdana_16.png"
0:

.align 4


.global Verdana_16_abc_size
.set Verdana_16_abc_size, 0f-Verdana_16_abc
.global Verdana_16_abc
Verdana_16_abc:
	.incbin "Verdana_16.abc"
0:

.align 4


.global bg_vertex_vsu_size
.set bg_vertex_vsu_size, 0f-bg_vertex_vsu
.global bg_vertex_vsu
bg_vertex_vsu:
	.incbin "bg_vertex.vsu"
0:

.align 4

.global bg_pixel_psu_size
.set bg_pixel_psu_size, 0f-bg_pixel_psu
.global bg_pixel_psu
bg_pixel_psu:
	.incbin "bg_pixel.psu"
0: