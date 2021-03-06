#include <stdint.h>

extern uint32_t currentOutputWidth;
extern uint32_t currentOutputHeight;

unsigned int get_time_in_milliseconds(void);

int steamcompmgr_main(int argc, char **argv);

#include "rendervulkan.hpp"

#include <mutex>
#include <vector>

#include <wlr/render/dmabuf.h>

#include <X11/extensions/Xfixes.h>

struct ResListEntry_t {
	struct wlr_surface *surf;
	struct wlr_dmabuf_attributes attribs;
};

struct _XDisplay;
struct _win;

class MouseCursor
{
public:
	explicit MouseCursor(_XDisplay *display);

	int x() const;
	int y() const;

	void move(int x, int y);
	void updatePosition();
	void constrainPosition();
	void resetPosition();

	void paint(struct _win *window, struct Composite_t *pComposite,
			   struct VulkanPipeline_t *pPipeline);
	void setDirty();

private:
	void warp(int x, int y);
	void checkSuspension();

	void queryGlobalPosition(int &x, int &y);
	void queryPositions(int &rootX, int &rootY, int &winX, int &winY);
	void queryButtonMask(unsigned int &mask);

	bool getTexture();

	int m_x = 0, m_y = 0;
	int m_hotspotX = 0, m_hotspotY = 0;
	int m_width = 0, m_height = 0;

	VulkanTexture_t m_texture = 0;
	bool m_dirty;
	bool m_imageEmpty;

	unsigned int m_lastMovedTime = 0;
	bool m_hideForMovement;

	PointerBarrier m_scaledFocusBarriers[4] = { None };

	bool m_hasPlane;

	_XDisplay *m_display;
};

extern std::mutex wayland_commit_lock;
extern std::vector<ResListEntry_t> wayland_commit_queue;

extern std::vector< wlr_surface * > wayland_surfaces_deleted;

extern float focusedWindowScaleX;
extern float focusedWindowScaleY;
extern float focusedWindowOffsetX;
extern float focusedWindowOffsetY;
