#ifndef __AUTOFS_H__
#define __AUTOFS_H__

enum ip_regulator_id {
	MEDIA1_SUBSYS_ID = 0,
	MEDIA2_SUBSYS_ID,
	VIVOBUS_ID,
	VCODECSUBSYS_ID,
	DSSSUBSYS_ID,
	VDEC_ID,
	VENC_ID,
	VENC_RT_ID,
	IPP_ID,
	ISPSUBSYS_ID,
	ISP_R8_ID,
	ASP_ID,
	G3D_ID,
};

struct ip_regulator_autofs {
	int ip_regulator_id;
	char *ip_regulator_name;
};

struct ip_regulator_autofs g_ip_autofs_list[] = {
	{ VIVOBUS_ID, "vivobus" },
	{ VCODECSUBSYS_ID, "vcodecbus" },
	{ DSSSUBSYS_ID, "dssbus" },
	{ VDEC_ID, "vdecbus" },
	{ VENC_ID, "vencbus" },
	{ ISPSUBSYS_ID, "ispbus" },
	{ ISP_R8_ID, "ispr8bus" },
};

#endif