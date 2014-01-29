/* xoreos - A reimplementation of BioWare's Aurora engine
 *
 * xoreos is the legal property of its developers, whose names can be
 * found in the AUTHORS file distributed with this source
 * distribution.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 3
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 *
 * The Infinity, Aurora, Odyssey, Eclipse and Lycium engines, Copyright (c) BioWare corp.
 * The Electron engine, Copyright (c) Obsidian Entertainment and BioWare corp.
 */

/** @file graphics/renderable.h
 *  Base class for renderable objects.
 */

#ifndef GRAPHICS_RENDERABLE_H
#define GRAPHICS_RENDERABLE_H

#include <list>

#include <OgrePrerequisites.h>

#include "common/ustring.h"

#include "graphics/controllers.h"

namespace Ogre {
	class SceneNode;
	class Animation;
}

namespace Graphics {

enum FadeDirection {
	kFadeDirectionIn    = kAnimationFunctionIncrease        , ///< Fade in.
	kFadeDirectionOut   = kAnimationFunctionDecrease        , ///< Fade out.
	kFadeDirectionInOut = kAnimationFunctionIncreaseDecrease, ///< Fade in, then out again.
	kFadeDirectionOutIn = kAnimationFunctionDecreaseIncrease  ///< Fade out, then in again.
};

class Renderable {
public:
	Renderable(const Common::UString &scene = "world");
	virtual ~Renderable();

	Ogre::SceneNode *getRootNode();

	bool isVisible() const;

	Common::UString getID() const;

	virtual void setVisible(bool visible);

	/** Set the base position. All positioning will be done relative to the base position. */
	void setBasePosition(float x, float y, float z);
	/** Set the base orientation. All orienting will be done relative to the base orientation. */
	void setBaseOrientation(float radian, float x, float y, float z);
	/** Set the base scale. All scaling will be done relative to the base scale. */
	void setBaseScale(float x, float y, float z);

	/** Get the size of the renderable. */
	void getSize(float &width, float &height, float &depth) const;

	/** Get the current position of the renderable. */
	virtual void getPosition(float &x, float &y, float &z) const;
	/** Get the current orientation of the renderable. */
	virtual void getOrientation(float &radian, float &x, float &y, float &z) const;
	/** Get the current scale of the renderable. */
	virtual void getScale(float &x, float &y, float &z) const;

	/** Set the current position of the renderable. */
	virtual void setPosition(float x, float y, float z);
	/** Set the current orientation of the renderable. */
	virtual void setOrientation(float radian, float x, float y, float z);
	/** Set the current scale of the renderable. */
	virtual void setScale(float x, float y, float z);

	/** Move the renderable, relative to its current position. */
	virtual void move  (float x, float y, float z);
	/** Rotate the renderable, relative to its current orientation. */
	virtual void rotate(float radian, float x, float y, float z);
	/** Scale the renderable, relative to its current scale. */
	virtual void scale(float x, float y, float z);

	/** Show/Hide the bouding box(es) of this renderable. */
	virtual void showBoundingBox(bool show);

	/** Change whether the renderable can be selected (picked) by the user. */
	virtual void setSelectable(bool selectable);

	/** Fade the renderable. */
	void fade(FadeDirection direction, float length, bool loop);
	/** Stop all fading. */
	void stopFade();

protected:
	Common::UString _scene;

	Ogre::SceneNode *_rootNode;

	bool _visible;
	bool _selectable;

	float _basePosition[3];
	float _baseOrientation[4];
	float _baseScale[3];

	Ogre::Controller<Ogre::Real> *_fader;


	void destroyAnimation(const Common::UString &name);
	void destroyAnimation(Ogre::Animation *anim);

	/** Collect all materials used in the renderable, optionally making them dynamic and/or transparent as well. */
	virtual void collectMaterials(std::list<Ogre::MaterialPtr> &materials, bool makeDynamic = false, bool makeTransparent = false) = 0;

private:
	void destroy();

	static void addBoundBox(Ogre::AxisAlignedBox &bound, const Ogre::SceneNode &node);
};

} // End of namespace Graphics

#endif // GRAPHICS_RENDERABLE_H