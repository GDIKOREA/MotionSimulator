//
// codemasters dirt 시리즈 하드웨어 세팅 설정 파일을 읽고, 관리한다.
// xml 파일을 읽고, 쓰기 가능하다. boost 라이브러리를 이용. 
//
#pragma once

#include <string>
using std::string;


class cHardwareSettingConfig
{
public:
	cHardwareSettingConfig();
	virtual ~cHardwareSettingConfig();

	bool Read(const string &fileName);
	bool Write(const string &fileName);


	// hardware_settings_config
	int m_version;
	string m_deviceId;

	// cpu
	bool m_parallelUpdateRender;
	string m_workerMapFile;
	int m_forceFeedbackProcessor;
	int m_dvdStorageProcessor;
	int m_dataSetMonitorProcessor;
	int m_renderProcessor;
	int m_updateProcessor;
	int m_fileStreamProcessor;

	// audio_card
	string m_mixing;

	// graphics_card
	bool m_forcedx9;
	bool m_stereo_enable;
	float m_stereo_separation;
	float m_stereo_convergence;
	int m_resolution_width;
	int m_resolution_height;
	string m_resolution_aspect;
	bool m_resolution_fullscreen;
	int m_resolution_vsync;
	string m_resolution_multisampling;
	int m_resolution_refreshRate;
	float m_resolution_gamma;

	// graphics_detail
	string m_graphics_detail_level;

	// shadow
	bool m_shadows_enable;
	int m_shadows_size;
	int m_shadows_maskQuality;

	// particles
	bool m_particles_enable;
	bool m_particles_wind;
	bool m_particles_dynamicRes;

	// crowd
	bool m_crowd_enable;
	int m_crowd_detail;

	// cloth
	bool m_cloth_enable;
	bool m_cloth_tessellation;

	// postprocess
	int m_postprocess_quality;

	// groundcover
	string m_groundcover_mode;
	bool m_groundcover_clutter;

	// objects
	float m_objects_lod;
	float m_objects_maxlod;

	// tree
	float m_tree_lod;
	float m_tree_maxlod;

	// vehicles
	int m_vehicles_characterQuality;
	int m_vehicles_lodQuality;
	
	// envmap
	int m_envmap_faces;
	int m_envmap_size;
	bool m_envmap_forceBilinear;

	// water
	bool m_water_update;
	int m_water_detail;
	bool m_water_tessellation;

	// mirrors
	bool m_mirrors_enabled;
	bool m_mirrors_forceBilinear;
	int m_mirrors_width;
	int m_mirrors_height;
	int m_mirrors_car_maxlod;
	float m_mirrors_car_culldist;

	// skidmarks
	bool m_skidmarks_enabled;
	
	// dynamic_ambient_occ
	bool m_dynamic_ambient_occ_enabled;
	int m_dynamic_ambient_occ_quality;

	// night_lighting
	bool m_night_lighting_volumes;
	int m_night_lighting_lights;
	bool m_night_lighting_shadows;

	// physics
	bool m_physic_environmentalDamage;
	bool m_physic_vehicleDamage;

	// input
	string m_input_device_type;

	// motion
	bool m_motion_enabled;
	string m_motion_ip;
	int m_motion_port;
	int m_motion_delay;
	int m_motion_extradata;
};
