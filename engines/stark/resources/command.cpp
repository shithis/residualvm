/* ResidualVM - A 3D game interpreter
 *
 * ResidualVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the AUTHORS
 * file distributed with this source distribution.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
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
 */

#include "engines/stark/resources/command.h"

#include "engines/stark/debug.h"

#include "engines/stark/formats/xrc.h"

#include "engines/stark/resources/animscript.h"
#include "engines/stark/resources/bookmark.h"
#include "engines/stark/resources/bonesmesh.h"
#include "engines/stark/resources/dialog.h"
#include "engines/stark/resources/floorfield.h"
#include "engines/stark/resources/fmv.h"
#include "engines/stark/resources/item.h"
#include "engines/stark/resources/knowledge.h"
#include "engines/stark/resources/script.h"
#include "engines/stark/resources/sound.h"
#include "engines/stark/resources/speech.h"
#include "engines/stark/resources/textureset.h"

#include "engines/stark/services/services.h"
#include "engines/stark/services/dialogplayer.h"
#include "engines/stark/services/resourceprovider.h"
#include "engines/stark/services/userinterface.h"

#include "common/random.h"

namespace Stark {
namespace Resources {

Command::~Command() {
}

Command::Command(Object *parent, byte subType, uint16 index, const Common::String &name) :
				Object(parent, subType, index, name) {
	_type = TYPE;
}

Command *Command::execute(uint32 callMode, Script *script) {
	switch (_subType) {
	case kCommandBegin:
		return opScriptBegin();
	case kScriptCall:
		return opScriptCall(script, _arguments[1].referenceValue, _arguments[2].intValue);
	case kDialogCall:
		return opDialogCall(script, _arguments[1].referenceValue, _arguments[2].intValue);
	case kSetInteractiveMode:
		return opSetInteractiveMode(_arguments[1].intValue);
	case kLocationGoTo:
		return opLocationGoTo(_arguments[0].stringValue, _arguments[1].stringValue, _arguments[2].referenceValue, _arguments[3].intValue);
	case kScriptPause:
		return opScriptPause(script, _arguments[1].referenceValue);
	case kWalkTo:
		return opWalkTo(_arguments[1].intValue, _arguments[2].referenceValue, _arguments[3].intValue);
	case kGameLoop:
		return opGameLoop(_arguments[1].intValue);
	case kScriptPauseRandom:
		return opPauseRandom(script, _arguments[1].referenceValue);
	case kExit2DLocation:
		return opExit2DLocation(script);
	case kGoto2DLocation:
		return opGoto2DLocation(_arguments[0].stringValue, _arguments[1].stringValue);
	case kRumbleScene:
		return opRumbleScene(_arguments[1].intValue, _arguments[2].intValue);
	case kFadeScene:
		return opFadeScene(_arguments[1].intValue, _arguments[2].intValue, _arguments[3].intValue);
	case kItem3DPlaceOn:
		return opItem3DPlaceOn(_arguments[1].referenceValue, _arguments[2].referenceValue);
	case kItem3DWalkTo:
		return opItem3DWalkTo(_arguments[1].referenceValue, _arguments[2].referenceValue, _arguments[3].intValue);
	case kItemLookAt:
		return opItemLookAt(_arguments[1].referenceValue, _arguments[2].referenceValue, _arguments[3].intValue, _arguments[4].intValue);
	case kItemEnable:
		return opItemEnable(_arguments[1].referenceValue, _arguments[2].intValue);
	case kItemSetActivity:
		return opItemSetActivity(_arguments[1].referenceValue, _arguments[2].intValue, _arguments[3].intValue);
	case kUseAnimHierarchy:
		return opUseAnimHierachy(_arguments[1].referenceValue);
	case kPlayAnimation:
		return opPlayAnimation(_arguments[1].referenceValue, _arguments[2].intValue);
	case kShowPlay:
		return opShowPlay(script, _arguments[1].referenceValue, _arguments[2].intValue);
	case kScriptEnable:
		return opScriptEnable(_arguments[1].referenceValue, _arguments[2].intValue);
	case kSetBoolean:
		return opSetBoolean(_arguments[1].referenceValue, _arguments[2].intValue);
	case kSetInteger:
		return opSetInteger(_arguments[1].referenceValue, _arguments[2].intValue);
	case kEnableFloorField:
		return opEnableFloorField(_arguments[1].referenceValue, _arguments[2].intValue);
	case kPlayAnimScriptItem:
		return opPlayAnimScriptItem(script, _arguments[1].referenceValue, _arguments[2].intValue);
	case kSoundPlay:
		return opSoundPlay(script, _arguments[1].referenceValue, _arguments[2].intValue);
	case kLookDirection:
		return opLookDirection(_arguments[1].referenceValue, _arguments[2].intValue, _arguments[3].intValue);
	case kStopPlayingSound:
		return opStopPlayingSound(_arguments[1].referenceValue);
	case kGoLayer:
		return opGoLayer(_arguments[1].referenceValue);
	case kScrollSet:
		return opScrollSet(_arguments[1].referenceValue);
	case kPlayFullMotionVideo:
		return opPlayFullMotionVideo(script, _arguments[1].referenceValue, _arguments[2].intValue);
	case kEnableDiaryEntry:
		return opEnableDiaryEntry(_arguments[1].referenceValue);
	case kChangeSound:
		return opChangeSound(_arguments[1].referenceValue, _arguments[2].intValue, _arguments[3].intValue, _arguments[4].intValue, _arguments[5].intValue);
	case kItem3DRunTo:
		return opItem3DRunTo(_arguments[1].referenceValue, _arguments[2].referenceValue, _arguments[3].intValue);
	case kItemPlaceDirection:
		return opItemPlaceDirection(_arguments[1].referenceValue, _arguments[2].intValue);
	case kActivateTexture:
		return opActivateTexture(_arguments[1].referenceValue);
	case kActivateMesh:
		return opActivateMesh(_arguments[1].referenceValue);
	case kSetTarget:
		return opSetTarget(_arguments[1].referenceValue, _arguments[2].referenceValue);
	case kSpeakWithoutTalking:
		return opSpeakWithoutTalking(script, _arguments[1].referenceValue, _arguments[2].intValue);
	case kIsOnFloorField:
		return opIsOnFloorField(_arguments[2].referenceValue, _arguments[3].referenceValue);
	case kIsItemEnabled:
		return opIsItemEnabled(_arguments[2].referenceValue);
	case kIsSet:
		return opIsSet(_arguments[2].referenceValue);
	case kIsIntegerInRange:
		return opIsIntegerInRange(_arguments[2].referenceValue, _arguments[3].intValue, _arguments[4].intValue);
	case kIsIntegerAbove:
		return opIsIntegerAbove(_arguments[2].referenceValue, _arguments[3].intValue);
	case kIsIntegerEqual:
		return opIsIntegerEqual(_arguments[2].referenceValue, _arguments[3].intValue);
	case kIsIntegerLower:
		return opIsIntegerLower(_arguments[2].referenceValue, _arguments[3].intValue);
	case kIsScriptActive:
		return opIsScriptActive(_arguments[2].referenceValue);
	case kIsRandom:
		return opIsRandom(_arguments[2].intValue);
	case kIsOnPlace:
		return opIsOnPlace(_arguments[0].intValue, _arguments[1].intValue, _arguments[2].referenceValue, _arguments[3].referenceValue);
	case kIsOnNearPlace:
		return opIsOnNearPlace(_arguments[0].intValue, _arguments[1].intValue, _arguments[2].referenceValue, _arguments[3].referenceValue, _arguments[4].intValue);
	case kIsAnimPlaying:
		return opIsAnimPlaying(_arguments[0].intValue, _arguments[1].intValue, _arguments[2].referenceValue);
	case kIsAnimAtTime:
		return opIsAnimAtTime(_arguments[0].intValue, _arguments[1].intValue, _arguments[2].referenceValue, _arguments[3].intValue);
	default:
		warning("Unimplemented command %d - %s", _subType, _name.c_str());
		printData();
		break;
	}

	return nextCommand();
}

Command *Command::opScriptBegin() {
	return nextCommand();
}

Command *Command::opScriptCall(Script *script, const ResourceReference &scriptRef, int32 synchronous) {
	Script *calleeScript = scriptRef.resolve<Script>();

	if (synchronous) {
		// Store the current command for use when the callee's execution ends
		script->addReturnObject(this);

		// Run the callee with the current script's execution context
		return calleeScript->getBeginCommand();
	} else {
		// Kickstart the callee script by skipping its Begin command, overriding all checks
		// Both scripts will continue in parallel
		calleeScript->goToNextCommand();
		return nextCommand();
	}
}

Command *Command::opDialogCall(Script *script, const ResourceReference &dialogRef, int32 suspend) {
	DialogPlayer *dialogPlayer = StarkServices::instance().dialogPlayer;

	Dialog *dialog = dialogRef.resolve<Dialog>();
	dialogPlayer->run(dialog);

	if (suspend) {
		script->suspend(dialog);
		return this; // Stay on the same command while suspended
	} else {
		return nextCommand();
	}
}

Command *Command::opSetInteractiveMode(bool enabled) {
	assert(_arguments.size() == 2);
	warning("(TODO: Implement) opSetInteractiveMode(%d)", enabled);
	return nextCommand();
}

Command *Command::opLocationGoTo(const Common::String &level, const Common::String &location, const ResourceReference &bookmarkRef, int32 direction) {
	ResourceProvider *resourceProvider = StarkServices::instance().resourceProvider;

	uint levelIndex = strtol(level.c_str(), nullptr, 16);
	uint locationIndex = strtol(location.c_str(), nullptr, 16);
	resourceProvider->requestLocationChange(levelIndex, locationIndex);
	resourceProvider->setNextLocationPosition(bookmarkRef, direction);

	return nullptr;
}


Command *Command::opScriptPause(Script *script, const ResourceReference &durationRef) {
	Knowledge *duration = durationRef.resolve<Knowledge>();
	script->pause(duration->getIntegerValue());

	return this; // Stay on this command while the script is suspended
}

Command *Command::opWalkTo(int32 unknown, const ResourceReference &bookmarkRef, int32 unknown2) {
	assert(_arguments.size() == 4);
	Object *target = bookmarkRef.resolve<Object>();
	warning("(TODO: Implement) opWalkTo(%d %s %d) : %s", unknown, target->getName().c_str(), unknown2, bookmarkRef.describe().c_str());

	return nextCommand();
}

Command *Command::opGameLoop(int32 unknown) {
	assert(_arguments.size() == 2);
	assert(_arguments[1].type == Argument::kTypeInteger1 || _arguments[1].type == Argument::kTypeInteger2);
	warning("(TODO: Implement) opGameLoop(%d)", unknown);

	return nextCommand();
}

Command *Command::opPauseRandom(Script *script, const ResourceReference &ref) {
	Common::RandomSource *randomSource = StarkServices::instance().randomSource;
	float randomFactor = randomSource->getRandomNumber(10000) / 10000.0;

	Knowledge *duration = ref.resolve<Knowledge>();
	script->pause(randomFactor * duration->getIntegerValue());

	return this; // Stay on this command while the script is suspended
}

Command *Command::opExit2DLocation(Script *script) {
	warning("(TODO: Implement) Exit 2D Location");
	StarkServices::instance().resourceProvider->returnToPushedLocation();
	return nullptr;
}

Command *Command::opGoto2DLocation(const Common::String &level, const Common::String &location) {
	ResourceProvider *resourceProvider = StarkServices::instance().resourceProvider;

	warning("TODO: Implement) opGoto2DLocation");
	// TODO: This needs to be handled differently, to allow exiting.
	uint levelIndex = strtol(level.c_str(), nullptr, 16);
	uint locationIndex = strtol(location.c_str(), nullptr, 16);
	resourceProvider->pushAndChangeLocation(levelIndex, locationIndex);
	return nullptr;
}

Command *Command::opRumbleScene(int32 unknown1, int32 unknown2) {
	warning("(TODO: Implement) opRumble(%d, %d)", unknown1, unknown2);

	return nextCommand();
}

Command *Command::opFadeScene(int32 unknown1, int32 unknown2, int32 unknown3) {
	warning("(TODO: Implement) opFadeScene(%d, %d, %d) : %s", unknown1, unknown2, unknown3, getName().c_str());

	return nextCommand();
}

Command *Command::opItem3DPlaceOn(const ResourceReference &itemRef, const ResourceReference &targetRef) {
	FloorPositionedItem *item = itemRef.resolve<FloorPositionedItem>();
	Object *target = targetRef.resolve<Object>();

	switch (target->getType().get()) {
	case Type::kBookmark:
		item->placeOnBookmark(Object::cast<Bookmark>(target));
		break;
	default:
		warning("Unimplemented op3DPlaceOn target type %s", target->getType().getName());
	}

	return nextCommand();
}

Command *Command::opItem3DWalkTo(const ResourceReference &itemRef, const ResourceReference &targetRef, int32 unknown) {
	assert(_arguments.size() == 4);
	Object *item = itemRef.resolve<Object>();
	Object *target = targetRef.resolve<Object>();
	warning("(TODO: Implement) opItem3DWalkTo(%s, %s, %d), %s : %s", item->getName().c_str(), target->getName().c_str(), unknown, itemRef.describe().c_str(), targetRef.describe().c_str());

	return nextCommand();
}

Command *Command::opItemLookAt(const ResourceReference &itemRef1, const ResourceReference &itemRef2, int32 unknown1, int32 unknown2) {
	assert(_arguments.size() == 5);
	Object *item1 = itemRef1.resolve<Object>();
	Object *item2 = itemRef2.resolve<Object>();
	warning("(TODO: Implement) opItemLookAt(%s, %s, %d, %d), %s : %s", item1->getName().c_str(), item2->getName().c_str(), unknown1, unknown2, itemRef1.describe().c_str(), itemRef2.describe().c_str());

	return nextCommand();
}

Command *Command::opItemEnable(const ResourceReference &itemRef, int32 enable) {
	Item *item = itemRef.resolve<Item>();

	bool previousState = item->isEnabled();

	switch (enable) {
	case 0:
		item->setEnabled(false);
		break;
	case 1:
		if (!previousState) {
			item->setEnabled(true);
		}
		break;
	case 2:
		item->setEnabled(!previousState);
		break;
	default:
		warning("Unhandled item enable command %d", enable);
		break;
	}

	return nextCommand();
}

Command *Command::opItemSetActivity(const ResourceReference &itemRef, int32 unknown1, int32 unknown2) {
	assert(_arguments.size() == 4);
	Object *itemObj = itemRef.resolve<Object>();
	warning("(TODO: Implement) opSetActivity(%s, %d, %d) : %s", itemObj->getName().c_str(), unknown1, unknown2, itemRef.describe().c_str());

	return nextCommand();
}

Command *Command::opUseAnimHierachy(const ResourceReference &animHierRef) {
	assert(_arguments.size() == 2);
	Object *animHier = animHierRef.resolve<Object>();
	warning("(TODO: Implement) opUseAnimHierarchy(%s) : %s", animHier->getName().c_str(), animHierRef.describe().c_str());

	return nextCommand();
}

Command *Command::opPlayAnimation(const ResourceReference &animRef, int32 unknown) {
	assert(_arguments.size() == 3);
	Object *anim = animRef.resolve<Object>();
	warning("(TODO: Implement) opPlayAnimation(%s, %d) : %s", anim->getName().c_str(), unknown, animRef.describe().c_str());

	return nextCommand();
}

Command *Command::opScriptEnable(const ResourceReference &scriptRef, int32 enable) {
	Script *script = scriptRef.resolve<Script>();

	bool previousState = script->isEnabled();

	switch (enable) {
		case 0:
			script->enable(false);
	        break;
		case 1:
			script->enable(true);
	        break;
		case 2:
			script->enable(!previousState);
	        break;
		default:
			warning("Unhandled script enable command %d", enable);
	        break;
	}

	return nextCommand();
}

Command *Command::opShowPlay(Script *script, const ResourceReference &ref, int32 suspend) {
	assert(_arguments.size() == 3);
	Speech *speechObj = ref.resolve<Speech>();
	assert(speechObj->getType().get() == Type::kSpeech);
	DialogPlayer *dialogPlayer = StarkServices::instance().dialogPlayer;
	dialogPlayer->playSingle(speechObj);
	warning("(TODO: Implement) opShowPlay(%s %d) %s : %s", speechObj->getName().c_str(), suspend, speechObj->getPhrase().c_str(), ref.describe().c_str());

	if (suspend) {
		script->suspend(speechObj);
		return this; // Stay on the same command while suspended
	} else {
		return nextCommand();
	}
}

Command *Command::opSetBoolean(const ResourceReference &knowledgeRef, int32 enable) {
	Knowledge *boolean = knowledgeRef.resolve<Knowledge>();

	bool previousState = boolean->getBooleanValue();

	switch (enable) {
		case 0:
			boolean->setBooleanValue(false);
	        break;
		case 1:
			boolean->setBooleanValue(true);
	        break;
		case 2:
			boolean->setBooleanValue(!previousState);
	        break;
		default:
			warning("Unhandled set boolean value command %d", enable);
	        break;
	}

	return nextCommand();
}

Command *Command::opSetInteger(const ResourceReference &knowledgeRef, int32 value) {
	Knowledge *integer = knowledgeRef.resolve<Knowledge>();

	integer->setIntegerValue(value);

	return nextCommand();
}

Command *Command::opEnableFloorField(const ResourceReference &floorFieldRef, int32 value) {
	assert(_arguments.size() == 3);
	Object *floorFieldObj = floorFieldRef.resolve<Object>();
	warning("(TODO: Implement) opEnableFloorField(%s, %d) : %s", floorFieldObj->getName().c_str(), value, floorFieldRef.describe().c_str());

	return nextCommand();
}

Command *Command::opPlayAnimScriptItem(Script *script, const ResourceReference &animScriptItemRef, int32 pause) {
	assert(_arguments.size() == 3);
	Object *animScriptItemObj = animScriptItemRef.resolve<Object>();

	AnimScript *animScript = animScriptItemObj->findParent<AnimScript>();
	assert(animScript);
	animScript->setCurrentIndex(animScriptItemObj->getIndex());
	// TODO: If pause is true, pause the script for the duration of the anim
	warning("(TODO: Implement) opPlayAnimScriptItem(%s, %d) : %s", animScriptItemObj->getName().c_str(), pause, animScriptItemRef.describe().c_str());
	return nextCommand();
}

Command *Command::opSoundPlay(Script *script, const ResourceReference &soundRef, int32 suspend) {
	Sound *sound = soundRef.resolve<Sound>();
	sound->play();

	if (suspend) {
		script->suspend(sound);
		return this; // Stay on the same command while suspended
	} else {
		return nextCommand();
	}
}

Command *Command::opLookDirection(const ResourceReference &itemRef, int32 unknown1, int32 unknown2) {
	assert(_arguments.size() == 4);
	Object *itemObj = itemRef.resolve<Object>();
	warning("(TODO: Implement) opLookDirection(%s %d %d) : %s", itemObj->getName().c_str(), unknown1, unknown2, itemRef.describe().c_str());

	return nextCommand();
}

Command *Command::opStopPlayingSound(const ResourceReference &soundRef) {
	Sound *soundObj = soundRef.resolve<Sound>();

	soundObj->stop();

	return nextCommand();
}

Command *Command::opGoLayer(const ResourceReference &layerRef) {
	Object *layerObj = layerRef.resolve<Object>();
	warning("(TODO: Implement) opGoLayer(%s) : %s", layerObj->getName().c_str(), layerRef.describe().c_str());

	return nextCommand();
}

Command *Command::opScrollSet(const ResourceReference &scrollRef) {
	assert(_arguments.size() == 2);
	Object *scroll =  scrollRef.resolve<Object>();
	warning("(TODO: Implement) opScrollSet(%s) : %s", scroll->getName().c_str(), scrollRef.describe().c_str());

	return nextCommand();
}

Command *Command::opPlayFullMotionVideo(Script *script, const ResourceReference &movieRef, int32 unknown) {
	FMV *movie =  movieRef.resolve<FMV>();
	warning("(TODO: Implement) opPlayFullMotionVideo(%s) : %s - %d", movie->getName().c_str(), movieRef.describe().c_str(), unknown);
	StarkServices::instance().userInterface->notifyFMVRequest(movie->getFilename());
	// TODO: Is this unconditional suspension?
	script->suspend(movie);
	return this; // Stay on the same command while suspended
}

Command *Command::opEnableDiaryEntry(const ResourceReference &knowledgeRef) {
	assert(_arguments.size() == 2);
	Knowledge *entry = knowledgeRef.resolve<Knowledge>();
	warning("(TODO: Implement) opEnableDiaryEntry(%s) : %s", entry->getName().c_str(), knowledgeRef.describe().c_str());

	return nextCommand();
}

Command *Command::opChangeSound(const ResourceReference &soundRef, int32 unknown1, int32 unknown2, int32 unknown3, int32 unknown4) {
	assert(_arguments.size() == 6);
	Object *sound = soundRef.resolve<Object>();
	warning("(TODO: Implement) opChangeSound(%s, %d, %d, %d, %d) : %s", sound->getName().c_str(), unknown1, unknown2, unknown3, unknown4, soundRef.describe().c_str());
	return nextCommand();
}

Command *Command::opItem3DRunTo(const ResourceReference &itemRef, const ResourceReference &bookmarkRef, int32 unknown) {
	assert(_arguments.size() == 4);
	Object *itemObj = itemRef.resolve<Object>();
	Object *bookmarkObj = bookmarkRef.resolve<Object>();

	warning("(TODO: Implement) opItem3DRunTo(%s, %s, %d) %s : %s", itemObj->getName().c_str(), bookmarkObj->getName().c_str(),unknown, itemRef.describe().c_str(), bookmarkRef.describe().c_str());

	return nextCommand();
}

Command *Command::opItemPlaceDirection(const ResourceReference &itemRef, int32 direction) {
	FloorPositionedItem *item = itemRef.resolve<FloorPositionedItem>();

	item->setDirection(abs(direction) % 360);

	return nextCommand();
}

Command *Command::opActivateTexture(const ResourceReference &textureRef) {
	TextureSet *texture = textureRef.resolve<TextureSet>();
	Item *item = texture->findParent<Item>();

	if (!item || (item->getSubType() != Item::kItemGlobalTemplate && item->getSubType() != Item::kItemLevelTemplate && item->getSubType() != Item::kItemMesh)) {
		return nextCommand();
	}

	if (item->getSubType() == Item::kItemMesh) {
		MeshItem *item10 = Object::cast<MeshItem>(item);
		item10->setTexture(texture->getIndex(), texture->getSubType());
	} else {
		ItemTemplate *item13 = Object::cast<ItemTemplate>(item);
		item13->setTexture(texture->getIndex(), texture->getSubType());
	}

	return nextCommand();
}

Command *Command::opActivateMesh(const ResourceReference &meshRef) {
	BonesMesh *mesh = meshRef.resolve<BonesMesh>();
	Item *item = mesh->findParent<Item>();

	if (!item || (item->getSubType() != Item::kItemGlobalTemplate && item->getSubType() != Item::kItemLevelTemplate && item->getSubType() != Item::kItemMesh)) {
		return nextCommand();
	}

	if (item->getSubType() == Item::kItemMesh) {
		MeshItem *item10 = Object::cast<MeshItem>(item);
		item10->setBonesMesh(mesh->getIndex());
	} else {
		ItemTemplate *item13 = Object::cast<ItemTemplate>(item);
		item13->setBonesMesh(mesh->getIndex());
	}

	return nextCommand();
}

Command *Command::opSetTarget(const ResourceReference &itemRef1, const ResourceReference &itemRef2) {
	Object *itemObj1 = itemRef1.resolve<Object>();
	Object *itemObj2 = itemRef2.resolve<Object>();
	warning("(TODO: Implement) opSetTarget(%s, %s) %s : %s", itemObj1->getName().c_str(), itemObj2->getName().c_str(), itemRef1.describe().c_str(), itemRef2.describe().c_str());

	return nextCommand();
}

Command *Command::opSpeakWithoutTalking(Script *script, const ResourceReference &speechRef, int32 suspend) {
	assert(_arguments.size() == 3);
	Speech *speech = speechRef.resolve<Speech>();
	warning("(TODO: Implement) opSpeakWithoutTalking(%s, %d) : %s", speech->getName().c_str(), suspend, speechRef.describe().c_str());

	// TODO: Complete
	DialogPlayer *dialogPlayer = StarkServices::instance().dialogPlayer;
	dialogPlayer->playSingle(speech);

	if (suspend) {
		script->suspend(speech);
		return this; // Stay on the same command while suspended
	} else {
		return nextCommand();
	}
}

Command *Command::opIsOnFloorField(const ResourceReference &itemRef, const ResourceReference &floorFieldRef) {
	FloorPositionedItem *item = itemRef.resolve<FloorPositionedItem>();
	FloorField *floorField = floorFieldRef.resolve<FloorField>();

	int32 itemFaceIndex = item->getFloorFaceIndex();
	bool itemOnFloorField = floorField->hasFace(itemFaceIndex);

	return nextCommandIf(itemOnFloorField);
}

Command *Command::opIsItemEnabled(const ResourceReference &itemRef) {
	Item *itemObj = itemRef.resolve<Item>();

	return nextCommandIf(itemObj->isEnabled());
}

Command *Command::opIsSet(const ResourceReference &knowledgeRef) {
	Knowledge *value = knowledgeRef.resolve<Knowledge>();

	return nextCommandIf(value->getBooleanValue());
}

Command *Command::opIsIntegerInRange(const ResourceReference &knowledgeRef, int32 min, int32 max) {
	Knowledge *knowledgeValue = knowledgeRef.resolve<Knowledge>();
	int value = knowledgeValue->getIntegerValue();

	return nextCommandIf(value >= min && value <= max);
}

Command *Command::opIsIntegerAbove(const ResourceReference &knowledgeRef, int32 value) {
	Knowledge *knowledgeValue = knowledgeRef.resolve<Knowledge>();

	return nextCommandIf(knowledgeValue->getIntegerValue() > value);
}

Command *Command::opIsIntegerEqual(const ResourceReference &knowledgeRef, int32 value) {
	Knowledge *knowledgeValue = knowledgeRef.resolve<Knowledge>();

	return nextCommandIf(knowledgeValue->getIntegerValue() == value);
}

Command *Command::opIsIntegerLower(const ResourceReference &knowledgeRef, int32 value) {
	Knowledge *knowledgeValue = knowledgeRef.resolve<Knowledge>();

	return nextCommandIf(knowledgeValue->getIntegerValue() < value);
}

Command *Command::opIsScriptActive(const ResourceReference &scriptRef) {
	Script *script = scriptRef.resolve<Script>();

	return nextCommandIf(!script->isOnBegin());
}

Command *Command::opIsRandom(int32 chance) {
	Common::RandomSource *randomSource = StarkServices::instance().randomSource;
	int32 value = randomSource->getRandomNumber(100);

	return nextCommandIf(value < chance);
}

Command *Command::opIsOnPlace(int branch1, int branch2, const ResourceReference &itemRef, const ResourceReference &position) {
	assert(_arguments.size() == 4);
	Object *itemObj = itemRef.resolve<Object>();
	Object *positionObj = position.resolve<Object>();
	warning("(TODO: Implement) opIsOnPlace(%d, %d, %s, %s) %s : %s", branch1, branch2, itemObj->getName().c_str(), positionObj->getName().c_str(), itemRef.describe().c_str(), position.describe().c_str());
	// TODO: Verify how this logic actually should be handled
	return nextCommandIf(false);
}

Command *Command::opIsOnNearPlace(int branch1, int branch2, const ResourceReference &itemRef, const ResourceReference &position, int32 unknown) {
	assert(_arguments.size() == 5);
	Object *itemObj = itemRef.resolve<Object>();
	Object *positionObj = position.resolve<Object>();
	warning("(TODO: Implement) opIsOnPlace(%d, %d, %s, %s, %d) %s : %s", branch1, branch2, itemObj->getName().c_str(), positionObj->getName().c_str(), unknown, itemRef.describe().c_str(), position.describe().c_str());
	// TODO: Verify how this logic actually should be handled
	return nextCommandIf(true);
}

Command *Command::opIsAnimPlaying(int branch1, int branch2, const ResourceReference &animRef) {
	assert(_arguments.size() == 3);
	Object *animObj = animRef.resolve<Object>();
	warning("(TODO: Implement opIsAnimPlaying(%d %d %s) %s", branch1, branch2, animObj->getName().c_str(), animRef.describe().c_str());

	// TODO: Just returning true, since we're skipping anims for now.
	return nextCommandIf(false);
}

Command *Command::opIsAnimAtTime(int branch1, int branch2, const ResourceReference &animRef, int32 time) {
	assert(_arguments.size() == 4);
	Object *animObj = animRef.resolve<Object>();
	warning("(TODO: Implement opIsAnimAtTime(%d %d %s %d) %s", branch1, branch2, animObj->getName().c_str(), time, animRef.describe().c_str());

	// TODO: Just returning true, since we're skipping anims for now.
	return nextCommandIf(true);
}

Command *Command::nextCommand() {
	assert(!_arguments.empty());
	assert(_arguments[0].type == Argument::kTypeInteger1);

	return resolveArgumentSiblingReference(_arguments[0]);
}

Command *Command::nextCommandIf(bool predicate) {
	assert(_arguments.size() >= 2);
	assert(_arguments[0].type == Argument::kTypeInteger1);
	assert(_arguments[1].type == Argument::kTypeInteger1);

	if (predicate) {
		return resolveArgumentSiblingReference(_arguments[1]);
	} else {
		return resolveArgumentSiblingReference(_arguments[0]);
	}
}

Command *Command::resolveArgumentSiblingReference(const Argument &argument) {
	return _parent->findChildWithIndex<Command>(argument.intValue);
}

void Command::readData(Formats::XRCReadStream *stream) {
	uint32 count = stream->readUint32LE();
	for (uint i = 0; i < count; i++) {
		Argument argument;
		argument.type = stream->readUint32LE();

		switch (argument.type) {
		case Argument::kTypeInteger1:
		case Argument::kTypeInteger2:
			argument.intValue = stream->readUint32LE();
			break;

		case Argument::kTypeResourceReference:
			argument.referenceValue = stream->readResourceReference();
			break;
		case Argument::kTypeString:
			argument.stringValue = stream->readString();
			break;
		default:
			error("Unknown argument type %d", argument.type);
		}

		_arguments.push_back(argument);
	}
}

void Command::printData() {
	for (uint i = 0; i < _arguments.size(); i++) {
		switch (_arguments[i].type) {
		case Argument::kTypeInteger1:
		case Argument::kTypeInteger2:
			debug("%d: %d", i, _arguments[i].intValue);
			break;

		case Argument::kTypeResourceReference: {
			debug("%d: %s", i, _arguments[i].referenceValue.describe().c_str());
		}
			break;
		case Argument::kTypeString:
			debug("%d: %s", i, _arguments[i].stringValue.c_str());
			break;
		default:
			error("Unknown argument type %d", _arguments[i].type);
		}
	}
}

} // End of namespace Resources
} // End of namespace Stark
