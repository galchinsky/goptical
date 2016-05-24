set(MODULE_SOURCES
  analysis_focus.cpp
  analysis_pointimage.cpp
  analysis_rayfan.cpp
  analysis_spot.cpp
  curve_array.cpp
  curve_base.cpp
  curve_composer.cpp
  curve_conic_base.cpp
  curve_conic.cpp
  curve_flat.cpp
  curve_foucault.cpp
  curve_grid.cpp
  curve_parabola.cpp
  curve_polynomial.cpp
  curve_rotational.cpp
  curve_sphere.cpp
  curve_spline.cpp
  curve_zernike.cpp
  data_discrete_set.cpp
  data_grid.cpp
  data_interpolate_1d_.hxx
  data_plot.cpp
  data_sample_set.cpp
  data_set1d.cpp
  data_set.cpp
  io_import_oslo.cpp
  io_import_zemax.cpp
  io_renderer_2d.cpp
  io_renderer_axes.cpp
  io_renderer.cpp
  io_renderer_dxf.cpp
  io_renderer_gd.cpp
  io_renderer_opengl.cpp
  io_renderer_plplot.cpp
  io_renderer_svg.cpp
  io_renderer_viewport.cpp
  io_renderer_x11.cpp
  io_renderer_x3d.cpp
  io_rgb.cpp
  light_ray.cpp
  light_spectral_line.cpp
  material_abbe.cpp
  material_air.cpp
  material_base.cpp
  material_catalog.cpp
  material_conrady.cpp
  material_dielectric.cpp
  material_dispersion_table.cpp
  material_herzberger.cpp
  material_metal.cpp
  material_mirror.cpp
  material_proxy.cpp
  material_schott.cpp
  material_sellmeier.cpp
  material_sellmeiermod.cpp
  material_vacuum.cpp
  math_matrix.cpp
  math_transform.cpp
  shape_base.cpp
  shape_composer.cpp
  shape_disk.cpp
  shape_ellipse.cpp
  shape_elliptical_ring.cpp
  shape_infinite.cpp
  shape_polygon.cpp
  shape_rectangle.cpp
  shape_regular_polygon.cpp
  shape_ring.cpp
  shape_round_.hxx
  sys_container.cpp
  sys_element.cpp
  sys_group.cpp
  sys_image.cpp
  sys_lens.cpp
  sys_mirror.cpp
  sys_optical_surface.cpp
  sys_source.cpp
  sys_source_point.cpp
  sys_source_rays.cpp
  sys_source_disk.cpp  
  sys_stop.cpp
  sys_surface.cpp
  sys_system.cpp
  trace_result.cpp
  trace_sequence.cpp
  trace_tracer.cpp
  )

set(MODULE_SOURCES_WITH_DIR "")
foreach(src_file ${MODULE_SOURCES})
list(APPEND MODULE_SOURCES_WITH_DIR ${CMAKE_CURRENT_SOURCE_DIR}/${src_file})
endforeach(src_file)

message(STATUS ${MODULE_SOURCES_WITH_DIR})

list(APPEND SOURCES ${MODULE_SOURCES_WITH_DIR})
set(SOURCES "${SOURCES}" PARENT_SCOPE)


  