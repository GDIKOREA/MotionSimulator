
#include "hardwaresettingconfig.h"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>



cHardwareSettingConfig::cHardwareSettingConfig()
{
}

cHardwareSettingConfig::~cHardwareSettingConfig()
{
}


// 설정파일 읽음
bool cHardwareSettingConfig::Read(const string &fileName)
{
	try
	{
		// boost property tree
		using boost::property_tree::ptree;
		ptree props;
		boost::property_tree::read_xml(fileName, props);

		// hardware_settings_config
		m_version = props.get<int>("hardware_settings_config.<xmlattr>.version", 56);
		m_deviceId = props.get<string>("hardware_settings_config.<xmlattr>.deviceId", "0x1380");

		// cpu
		m_parallelUpdateRender = props.get<bool>("hardware_settings_config.cpu.threadStrategy.<xmlattr>.parallelUpdateRender", true);
		m_workerMapFile = props.get<string>("hardware_settings_config.cpu.threadStrategy.<xmlattr>.workerMapFile", "system/workerMap8Core.xml");
		m_forceFeedbackProcessor = props.get<int>("hardware_settings_config.cpu.threadStrategy.<xmlattr>.forceFeedbackProcessor", 6);
		m_dvdStorageProcessor = props.get<int>("hardware_settings_config.cpu.threadStrategy.<xmlattr>.dvdStorageProcessor", 7);
		m_dataSetMonitorProcessor = props.get<int>("hardware_settings_config.cpu.threadStrategy.<xmlattr>.dataSetMonitorProcessor", 4);
		m_renderProcessor = props.get<int>("hardware_settings_config.cpu.threadStrategy.<xmlattr>.renderProcessor", 0);
		m_updateProcessor = props.get<int>("hardware_settings_config.cpu.threadStrategy.<xmlattr>.updateProcessor", 2);
		m_fileStreamProcessor = props.get<int>("hardware_settings_config.cpu.threadStrategy.<xmlattr>.fileStreamProcessor", 5);

		// audio_card
		m_mixing = props.get<string>("hardware_settings_config.audio_card.audio.<xmlattr>.mixing", "software");

		// graphics_card
		m_forcedx9 = props.get<bool>("hardware_settings_config.graphics_card.directx.<xmlattr>.forcedx9", false);
		m_stereo_enable = props.get<bool>("hardware_settings_config.graphics_card.stereo.<xmlattr>.enabled", false);
		m_stereo_separation = props.get<float>("hardware_settings_config.graphics_card.stereo.<xmlattr>.separation", 0.015f);
		m_stereo_convergence = props.get<float>("hardware_settings_config.graphics_card.stereo.<xmlattr>.convergence", 1.5f);
		m_resolution_width = props.get<int>("hardware_settings_config.graphics_card.resolution.<xmlattr>.width", 1920);
		m_resolution_height = props.get<int>("hardware_settings_config.graphics_card.resolution.<xmlattr>.height", 1080);
		m_resolution_aspect = props.get<string>("hardware_settings_config.graphics_card.resolution.<xmlattr>.aspect", "auto");
		m_resolution_fullscreen = props.get<bool>("hardware_settings_config.graphics_card.resolution.<xmlattr>.fullscreen", true);
		m_resolution_vsync = props.get<int>("hardware_settings_config.graphics_card.resolution.<xmlattr>.vsync", 0);
		m_resolution_multisampling = props.get<string>("hardware_settings_config.graphics_card.resolution.<xmlattr>.multisampling", "4xmsaa");
		m_resolution_refreshRate = props.get<int>("hardware_settings_config.graphics_card.resolution.refreshRate.<xmlattr>.rate", 60);
		m_resolution_gamma = props.get<float>("hardware_settings_config.graphics_card.gamma.<xmlattr>.level", 1.f);

		// graphics_detail
		m_graphics_detail_level = props.get<string>("hardware_settings_config.graphics_detail.<xmlattr>.level", "high");

		// shadow
		m_shadows_enable = props.get<bool>("hardware_settings_config.shadows.<xmlattr>.enabled", true);
		m_shadows_size = props.get<int>("hardware_settings_config.shadows.<xmlattr>.size", 2048);
		m_shadows_maskQuality = props.get<int>("hardware_settings_config.shadows.<xmlattr>.maskQuality", 1);

		// particles
		m_particles_enable = props.get<bool>("hardware_settings_config.particles.<xmlattr>.enabled", true);
		m_particles_wind = props.get<bool>("hardware_settings_config.particles.<xmlattr>.wind", true);
		m_particles_dynamicRes = props.get<bool>("hardware_settings_config.particles.<xmlattr>.dynamicRes", false);

		// crowd
		m_crowd_enable = props.get<bool>("hardware_settings_config.crowd.<xmlattr>.enabled", true);
		m_crowd_detail = props.get<int>("hardware_settings_config.crowd.<xmlattr>.detail", 1);

		// cloth
		m_cloth_enable = props.get<bool>("hardware_settings_config.cloth.<xmlattr>.enabled", true);
		m_cloth_tessellation = props.get<bool>("hardware_settings_config.cloth.<xmlattr>.tessellation", true);

		// postprocess
		m_postprocess_quality = props.get<int>("hardware_settings_config.postprocess.<xmlattr>.quality", 1);

		// groundcover
		m_groundcover_mode = props.get<string>("hardware_settings_config.groundcover.<xmlattr>.mode", "blended");
		m_groundcover_clutter = props.get<bool>("hardware_settings_config.groundcover.<xmlattr>.clutter", true);

		// objects
		m_objects_lod = props.get<float>("hardware_settings_config.objects.<xmlattr>.lod", 0.25f);
		m_objects_maxlod = props.get<float>("hardware_settings_config.objects.<xmlattr>.maxlod", 0.f);

		// tree
		m_tree_lod = props.get<float>("hardware_settings_config.trees.<xmlattr>.lod", 0.25f);
		m_tree_maxlod = props.get<float>("hardware_settings_config.trees.<xmlattr>.maxlod", 0.f);

		// vehicles
		m_vehicles_characterQuality = props.get<int>("hardware_settings_config.vehicles.<xmlattr>.characterQuality", 2);
		m_vehicles_lodQuality = props.get<int>("hardware_settings_config.vehicles.<xmlattr>.lodQuality", 2);

		// envmap
		m_envmap_faces = props.get<int>("hardware_settings_config.envmap.<xmlattr>.faces", 2);
		m_envmap_size = props.get<int>("hardware_settings_config.envmap.<xmlattr>.size", 512);
		m_envmap_forceBilinear = props.get<bool>("hardware_settings_config.envmap.<xmlattr>.forceBilinear", true);

		// water
		m_water_update = props.get<bool>("hardware_settings_config.water.<xmlattr>.update", true);
		m_water_detail = props.get<int>("hardware_settings_config.water.<xmlattr>.detail", 2);
		m_water_tessellation = props.get<bool>("hardware_settings_config.water.<xmlattr>.tessellation", true);

		// mirrors
		m_mirrors_enabled = props.get<bool>("hardware_settings_config.mirrors.<xmlattr>.enabled", true);
		m_mirrors_forceBilinear = props.get<bool>("hardware_settings_config.mirrors.<xmlattr>.forceBilinear", true);
		m_mirrors_width = props.get<int>("hardware_settings_config.mirrors.<xmlattr>.width", 768);
		m_mirrors_height = props.get<int>("hardware_settings_config.mirrors.<xmlattr>.height", 256);
		m_mirrors_car_maxlod = props.get<int>("hardware_settings_config.mirrors.<xmlattr>.car_maxlod", 1);
		m_mirrors_car_culldist = props.get<float>("hardware_settings_config.mirrors.<xmlattr>.car_culldist", 300.f);

		// skidmarks
		m_skidmarks_enabled = props.get<bool>("hardware_settings_config.skidmarks.<xmlattr>.enabled", true);

		// dynamic_ambient_occ
		m_dynamic_ambient_occ_enabled = props.get<bool>("hardware_settings_config.dynamic_ambient_occ.<xmlattr>.enabled", true);
		m_dynamic_ambient_occ_quality = props.get<int>("hardware_settings_config.dynamic_ambient_occ.<xmlattr>.quality", 1);

		// night_lighting
		m_night_lighting_volumes = props.get<bool>("hardware_settings_config.night_lighting.<xmlattr>.volumes", true);
		m_night_lighting_lights = props.get<int>("hardware_settings_config.night_lighting.<xmlattr>.lights", 0);
		m_night_lighting_shadows = props.get<bool>("hardware_settings_config.night_lighting.<xmlattr>.shadows", true);

		// physics
		m_physic_environmentalDamage = props.get<bool>("hardware_settings_config.physics.<xmlattr>.environmentalDamage", true);
		m_physic_vehicleDamage = props.get<bool>("hardware_settings_config.physics.<xmlattr>.vehicleDamage", true);

		// input
		m_input_device_type = props.get<string>("hardware_settings_config.input.<xmlattr>.device_type", "auto");

		// motion
		m_motion_enabled = props.get<bool>("hardware_settings_config.motion.<xmlattr>.enabled", true);
		m_motion_ip = props.get<string>("hardware_settings_config.motion.<xmlattr>.ip", "127.0.0.1");
		m_motion_port = props.get<int>("hardware_settings_config.motion.<xmlattr>.port", 20777);
		m_motion_delay = props.get<int>("hardware_settings_config.motion.<xmlattr>.delay", 1);
		m_motion_extradata = props.get<int>("hardware_settings_config.motion.<xmlattr>.extradata", 0);
	}
	catch (std::exception&)
	{
		//cout << "Read Error " << e.what() << endl;
		//::AfxMessageBox(CString(L"Error!!\n") + str2wstr(e.what()).c_str());
		return false;
	}

	return true;
}


// 설정파일을 저장한다.
bool cHardwareSettingConfig::Write(const string &fileName)
{

	try
	{
		// boost property tree
		using boost::property_tree::ptree;
		ptree props;

		// hardware_settings_config
		props.put<int>("hardware_settings_config.<xmlattr>.version", m_version);
		props.put<string>("hardware_settings_config.<xmlattr>.deviceId", m_deviceId);

		// cpu
		props.put<bool>("hardware_settings_config.cpu.threadStrategy.<xmlattr>.parallelUpdateRender", m_parallelUpdateRender);
		props.put<string>("hardware_settings_config.cpu.threadStrategy.<xmlattr>.workerMapFile", m_workerMapFile);
		props.put<int>("hardware_settings_config.cpu.threadStrategy.<xmlattr>.forceFeedbackProcessor", m_forceFeedbackProcessor);
		props.put<int>("hardware_settings_config.cpu.threadStrategy.<xmlattr>.dvdStorageProcessor", m_dvdStorageProcessor);
		props.put<int>("hardware_settings_config.cpu.threadStrategy.<xmlattr>.dataSetMonitorProcessor", m_dataSetMonitorProcessor);
		props.put<int>("hardware_settings_config.cpu.threadStrategy.<xmlattr>.renderProcessor", m_renderProcessor);
		props.put<int>("hardware_settings_config.cpu.threadStrategy.<xmlattr>.updateProcessor", m_updateProcessor);
		props.put<int>("hardware_settings_config.cpu.threadStrategy.<xmlattr>.fileStreamProcessor", m_fileStreamProcessor);

		// audio_card
		props.put<string>("hardware_settings_config.audio_card.audio.<xmlattr>.mixing", m_mixing);

		// graphics_card
		props.put<bool>("hardware_settings_config.graphics_card.directx.<xmlattr>.forcedx9", m_forcedx9);
		props.put<bool>("hardware_settings_config.graphics_card.stereo.<xmlattr>.enabled", m_stereo_enable);
		props.put<float>("hardware_settings_config.graphics_card.stereo.<xmlattr>.separation", m_stereo_separation);
		props.put<float>("hardware_settings_config.graphics_card.stereo.<xmlattr>.convergence", m_stereo_convergence);
		props.put<int>("hardware_settings_config.graphics_card.resolution.<xmlattr>.width", m_resolution_width);
		props.put<int>("hardware_settings_config.graphics_card.resolution.<xmlattr>.height", m_resolution_height);
		props.put<string>("hardware_settings_config.graphics_card.resolution.<xmlattr>.aspect", m_resolution_aspect);
		props.put<bool>("hardware_settings_config.graphics_card.resolution.<xmlattr>.fullscreen", m_resolution_fullscreen);
		props.put<int>("hardware_settings_config.graphics_card.resolution.<xmlattr>.vsync", m_resolution_vsync);
		props.put<string>("hardware_settings_config.graphics_card.resolution.<xmlattr>.multisampling", m_resolution_multisampling);
		props.put<int>("hardware_settings_config.graphics_card.resolution.refreshRate.<xmlattr>.rate", m_resolution_refreshRate);
		props.put<float>("hardware_settings_config.graphics_card.gamma.<xmlattr>.level", m_resolution_gamma);

		// graphics_detail
		props.put<string>("hardware_settings_config.graphics_detail.<xmlattr>.level", m_graphics_detail_level);

		// shadow
		props.put<bool>("hardware_settings_config.shadows.<xmlattr>.enabled", m_shadows_enable);
		props.put<int>("hardware_settings_config.shadows.<xmlattr>.size", m_shadows_size);
		props.put<int>("hardware_settings_config.shadows.<xmlattr>.maskQuality", m_shadows_maskQuality);

		// particles
		props.put<bool>("hardware_settings_config.particles.<xmlattr>.enabled", m_particles_enable);
		props.put<bool>("hardware_settings_config.particles.<xmlattr>.wind", m_particles_wind);
		props.put<bool>("hardware_settings_config.particles.<xmlattr>.dynamicRes", m_particles_dynamicRes);

		// crowd
		props.put<bool>("hardware_settings_config.crowd.<xmlattr>.enabled", m_crowd_enable);
		props.put<int>("hardware_settings_config.crowd.<xmlattr>.detail", m_crowd_detail);

		// cloth
		props.put<bool>("hardware_settings_config.cloth.<xmlattr>.enabled", m_cloth_enable);
		props.put<bool>("hardware_settings_config.cloth.<xmlattr>.tessellation", m_cloth_tessellation);

		// postprocess
		props.put<int>("hardware_settings_config.postprocess.<xmlattr>.quality", m_postprocess_quality);

		// groundcover
		props.put<string>("hardware_settings_config.groundcover.<xmlattr>.mode", m_groundcover_mode);
		props.put<bool>("hardware_settings_config.groundcover.<xmlattr>.clutter", m_groundcover_clutter);

		// objects
		props.put<float>("hardware_settings_config.objects.<xmlattr>.lod", m_objects_lod);
		props.put<float>("hardware_settings_config.objects.<xmlattr>.maxlod", m_objects_maxlod);

		// tree
		props.put<float>("hardware_settings_config.trees.<xmlattr>.lod", m_tree_lod);
		props.put<float>("hardware_settings_config.trees.<xmlattr>.maxlod", m_tree_maxlod);

		// vehicles
		props.put<int>("hardware_settings_config.vehicles.<xmlattr>.characterQuality", m_vehicles_characterQuality);
		props.put<int>("hardware_settings_config.vehicles.<xmlattr>.lodQuality", m_vehicles_lodQuality);

		// envmap
		props.put<int>("hardware_settings_config.envmap.<xmlattr>.faces", m_envmap_faces);
		props.put<int>("hardware_settings_config.envmap.<xmlattr>.size", m_envmap_size);
		props.put<bool>("hardware_settings_config.envmap.<xmlattr>.forceBilinear", m_envmap_forceBilinear);

		// water
		props.put<bool>("hardware_settings_config.water.<xmlattr>.update", m_water_update);
		props.put<int>("hardware_settings_config.water.<xmlattr>.detail", m_water_detail);
		props.put<bool>("hardware_settings_config.water.<xmlattr>.tessellation", m_water_tessellation);

		// mirrors
		props.put<bool>("hardware_settings_config.mirrors.<xmlattr>.enabled", m_mirrors_enabled);
		props.put<bool>("hardware_settings_config.mirrors.<xmlattr>.forceBilinear", m_mirrors_forceBilinear);
		props.put<int>("hardware_settings_config.mirrors.<xmlattr>.width", m_mirrors_width);
		props.put<int>("hardware_settings_config.mirrors.<xmlattr>.height", m_mirrors_height);
		props.put<int>("hardware_settings_config.mirrors.<xmlattr>.car_maxlod", m_mirrors_car_maxlod);
		props.put<float>("hardware_settings_config.mirrors.<xmlattr>.car_culldist", m_mirrors_car_culldist);

		// skidmarks
		props.put<bool>("hardware_settings_config.skidmarks.<xmlattr>.enabled", m_skidmarks_enabled);

		// dynamic_ambient_occ
		props.put<bool>("hardware_settings_config.dynamic_ambient_occ.<xmlattr>.enabled", m_dynamic_ambient_occ_enabled);
		props.put<int>("hardware_settings_config.dynamic_ambient_occ.<xmlattr>.quality", m_dynamic_ambient_occ_quality);

		// night_lighting
		props.put<bool>("hardware_settings_config.night_lighting.<xmlattr>.volumes", m_night_lighting_volumes);
		props.put<int>("hardware_settings_config.night_lighting.<xmlattr>.lights", m_night_lighting_lights);
		props.put<bool>("hardware_settings_config.night_lighting.<xmlattr>.shadows", m_night_lighting_shadows);

		// physics
		props.put<bool>("hardware_settings_config.physics.<xmlattr>.environmentalDamage", m_physic_environmentalDamage);
		props.put<bool>("hardware_settings_config.physics.<xmlattr>.vehicleDamage", m_physic_vehicleDamage);

		// input
		props.put<string>("hardware_settings_config.input.<xmlattr>.device_type", m_input_device_type);

		// motion
		props.put<bool>("hardware_settings_config.motion.<xmlattr>.enabled", m_motion_enabled);
		props.put<string>("hardware_settings_config.motion.<xmlattr>.ip", m_motion_ip);
		props.put<int>("hardware_settings_config.motion.<xmlattr>.port", m_motion_port);
		props.put<int>("hardware_settings_config.motion.<xmlattr>.delay", m_motion_delay);
		props.put<int>("hardware_settings_config.motion.<xmlattr>.extradata", m_motion_extradata);


		boost::property_tree::xml_writer_settings<string> settings('\t', 1);
		boost::property_tree::write_xml(fileName, props, std::locale(), settings);
	}
	catch (std::exception&)
	{
		//cout << "Read Error " << e.what() << endl;
		//::AfxMessageBox(CString(L"Error!!\n") + str2wstr(e.what()).c_str());
		return false;
	}

	return true;
}
