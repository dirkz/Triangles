set(SHADERS
	basic_triangle.hlsl
	basic_triangle_uniform.hlsl
	textured_quad.hlsl)

add_custom_target(shaders)

function(create_shader source stage target)
	string(TOLOWER ${target} target_lc)

	set(SHADER_SRC ${CMAKE_CURRENT_SOURCE_DIR}/${source})
	set(SHADER_OUTPUT_FILENAME ${source}.${stage}.${target_lc})
	set(SHADER_TARGET ${CMAKE_CURRENT_BINARY_DIR}/${SHADER_OUTPUT_FILENAME})

	if(${stage} STREQUAL "vertex")
		set (ENTRY "VS")
	elseif(${stage} STREQUAL "fragment")
		set (ENTRY "PS")
	else()
		set (ENTRY "CS")
	endif()

	add_custom_command(
		OUTPUT ${SHADER_TARGET}
		DEPENDS ${source}
		COMMAND shadercross -s HLSL -d ${target} -t ${stage} -e ${ENTRY} ${SHADER_SRC} -o ${SHADER_TARGET} 
	)

	target_sources(shaders PRIVATE ${SHADER_TARGET})
endfunction()

set(STAGES vertex fragment)
set(TARGETS DXIL SPIRV MSL)

foreach(SHADER ${SHADERS})
	foreach(STAGE ${STAGES})
		foreach(TARGET ${TARGETS})
			create_shader(${SHADER} ${STAGE} ${TARGET})
		endforeach()
	endforeach()
endforeach()
