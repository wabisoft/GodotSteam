#pragma once

#include "steam/steam_api.h"

#define STEAM_ENUM_DECLS()															\
enum AccountType {																	\
    ACCOUNT_TYPE_INVALID = 0,														\
    ACCOUNT_TYPE_INDIVIDUAL = 1,													\
    ACCOUNT_TYPE_MULTISEAT = 2,														\
    ACCOUNT_TYPE_GAME_SERVER = 3,													\
    ACCOUNT_TYPE_ANON_GAME_SERVER = 4,												\
    ACCOUNT_TYPE_PENDING = 5,														\
    ACCOUNT_TYPE_CONTENT_SERVER = 6,												\
    ACCOUNT_TYPE_CLAN = 7,															\
    ACCOUNT_TYPE_CHAT = 8,															\
    ACCOUNT_TYPE_CONSOLE_USER = 9,													\
    ACCOUNT_TYPE_ANON_USER = 10,													\
    ACCOUNT_TYPE_MAX = 11															\
};																					\
enum AppOwnershipFlags {															\
    APP_OWNERSHIP_FLAGS_NONE = 0x0000,												\
    APP_OWNERSHIP_FLAGS_OWNS_LICENSE = 0x0001,										\
    APP_OWNERSHIP_FLAGS_FREE_LICENSE = 0x0002,										\
    APP_OWNERSHIP_FLAGS_REGION_RESTRICTED = 0x0004,									\
    APP_OWNERSHIP_FLAGS_LOW_VIOLENCE = 0x0008,										\
    APP_OWNERSHIP_FLAGS_INVALID_PLATFORM = 0x0010,									\
    APP_OWNERSHIP_FLAGS_SHARED_LICENSE = 0x0020,									\
    APP_OWNERSHIP_FLAGS_FREE_WEEKEND = 0x0040,										\
    APP_OWNERSHIP_FLAGS_RETAIL_LICENSE = 0x0080,									\
    APP_OWNERSHIP_FLAGS_LICENSE_LOCKED = 0x0100,									\
    APP_OWNERSHIP_FLAGS_LICENSE_PENDING = 0x0200,									\
    APP_OWNERSHIP_FLAGS_LICENSE_EXPIRED = 0x0400,									\
    APP_OWNERSHIP_FLAGS_LICENSE_PERMANENT = 0x0800,									\
    APP_OWNERSHIP_FLAGS_LICENSE_RECURRING = 0x1000,									\
    APP_OWNERSHIP_FLAGS_LICENSE_CANCELED = 0x2000,									\
    APP_OWNERSHIP_FLAGS_AUTO_GRANT = 0x4000,										\
    APP_OWNERSHIP_FLAGS_PENDING_GIFT = 0x8000,										\
    APP_OWNERSHIP_FLAGS_RENTAL_NOT_ACTIVATED = 0x10000,								\
    APP_OWNERSHIP_FLAGS_RENTAL = 0x20000,											\
    APP_OWNERSHIP_FLAGS_SITE_LICENSE = 0x40000										\
};																					\
enum AppReleaseState {																\
    APP_RELEASE_STATE_UNKNOWN = 0,													\
    APP_RELEASE_STATE_UNAVAILABLE = 1,												\
    APP_RELEASE_STATE_PRERELEASE = 2,												\
    APP_RELEASE_STATE_PRELOAD_ONLY = 3,												\
    APP_RELEASE_STATE_RELEASED = 4													\
};																					\
enum AppType {																		\
    APP_TYPE_INVALID = 0x000,														\
    APP_TYPE_GAME = 0x001,															\
    APP_TYPE_APPLICATION = 0x002,													\
    APP_TYPE_TOOL = 0x004,															\
    APP_TYPE_DEMO = 0x008,															\
    APP_TYPE_MEDIA_DEPRECATED = 0x010,												\
    APP_TYPE_DLC = 0x020,															\
    APP_TYPE_GUIDE = 0x040,															\
    APP_TYPE_DRIVER = 0x080,														\
    APP_TYPE_CONFIG = 0x100,														\
    APP_TYPE_HARDWARE = 0x200,														\
    APP_TYPE_FRANCHISE = 0x400,														\
    APP_TYPE_VIDEO = 0x800,															\
    APP_TYPE_PLUGIN = 0x1000,														\
    APP_TYPE_MUSIC = 0x2000,														\
    APP_TYPE_SERIES = 0x4000,														\
    APP_TYPE_SHORTCUT = 0x40000000,													\
    APP_TYPE_DEPOT_ONLY = 0X80000000												\
};																					\
enum AuthSessionResponse {															\
    AUTH_SESSION_RESPONSE_OK = 0,													\
    AUTH_SESSION_RESPONSE_USER_NOT_CONNECTED_TO_STEAM = 1,							\
    AUTH_SESSION_RESPONSE_NO_LICENSE_OR_EXPIRED = 2,								\
    AUTH_SESSION_RESPONSE_VAC_BANNED = 3,											\
    AUTH_SESSION_RESPONSE_LOGGED_IN_ELSEWHERE = 4,									\
    AUTH_SESSION_RESPONSE_VAC_CHECK_TIMEDOUT = 5,									\
    AUTH_SESSION_RESPONSE_AUTH_TICKET_CANCELED = 6,									\
    AUTH_SESSION_RESPONSE_AUTH_TICKET_INVALID_ALREADY_USED = 7,						\
    AUTH_SESSION_RESPONSE_AUTH_TICKET_INVALID = 8,									\
    AUTH_SESSION_RESPONSE_PUBLISHER_ISSUED_BAN = 9									\
};																					\
enum BeginAuthSessionResult {														\
    BEGIN_AUTH_SESSION_RESULT_OK = 0,												\
    BEGIN_AUTH_SESSION_RESULT_INVALID_TICKET = 1,									\
    BEGIN_AUTH_SESSION_RESULT_DUPLICATE_REQUEST = 2,								\
    BEGIN_AUTH_SESSION_RESULT_INVALID_VERSION = 3,									\
    BEGIN_AUTH_SESSION_RESULT_GAME_MISMATCH = 4,									\
    BEGIN_AUTH_SESSION_RESULT_EXPIRED_TICKET = 5									\
};																					\
enum BroadcastUploadResult {														\
    BROADCAST_UPLOAD_RESULT_NONE = 0,												\
    BROADCAST_UPLOAD_RESULT_OK = 1,													\
    BROADCAST_UPLOAD_RESULT_INIT_FAILED = 2,										\
    BROADCAST_UPLOAD_RESULT_FRAME_FAILED = 3,										\
    BROADCAST_UPLOAD_RESULT_TIME_OUT = 4,											\
    BROADCAST_UPLOAD_RESULT_BANDWIDTH_EXCEEDED = 5,									\
    BROADCAST_UPLOAD_RESULT_LOW_FPS = 6,											\
    BROADCAST_UPLOAD_RESULT_MISSING_KEYFRAMES = 7,									\
    BROADCAST_UPLOAD_RESULT_NO_CONNECTION = 8,										\
    BROADCAST_UPLOAD_RESULT_RELAY_FAILED = 9,										\
    BROADCAST_UPLOAD_RESULT_SETTINGS_CHANGED = 10,									\
    BROADCAST_UPLOAD_RESULT_MISSING_AUDIO = 11,										\
    BROADCAST_UPLOAD_RESULT_TOO_FAR_BEHIND = 12,									\
    BROADCAST_UPLOAD_RESULT_TRANSCODE_BEHIND = 13									\
};																					\
enum ChatEntryType {																\
    CHAT_ENTRY_TYPE_INVALID = 0,													\
    CHAT_ENTRY_TYPE_CHAT_MSG = 1,													\
    CHAT_ENTRY_TYPE_TYPING = 2,														\
    CHAT_ENTRY_TYPE_INVITE_GAME = 3,												\
    CHAT_ENTRY_TYPE_EMOTE = 4,														\
    CHAT_ENTRY_TYPE_LEFT_CONVERSATION = 6,											\
    CHAT_ENTRY_TYPE_ENTERED = 7,													\
    CHAT_ENTRY_TYPE_WAS_KICKED = 8,													\
    CHAT_ENTRY_TYPE_WAS_BANNED = 9,													\
    CHAT_ENTRY_TYPE_DISCONNECTED = 10,												\
    CHAT_ENTRY_TYPE_HISTORICAL_CHAT = 11,											\
    CHAT_ENTRY_TYPE_LINK_BLOCKED = 14												\
};																					\
enum ChatRoomEnterResponse {														\
    CHAT_ROOM_ENTER_RESPONSE_SUCCESS = 1,											\
    CHAT_ROOM_ENTER_RESPONSE_DOESNT_EXIST = 2,										\
    CHAT_ROOM_ENTER_RESPONSE_NOT_ALLOWED = 3,										\
    CHAT_ROOM_ENTER_RESPONSE_FULL = 4,												\
    CHAT_ROOM_ENTER_RESPONSE_ERROR = 5,												\
    CHAT_ROOM_ENTER_RESPONSE_BANNED = 6,											\
    CHAT_ROOM_ENTER_RESPONSE_LIMITED = 7,											\
    CHAT_ROOM_ENTER_RESPONSE_CLAN_DISABLED = 8,										\
    CHAT_ROOM_ENTER_RESPONSE_COMMUNITY_BAN = 9,										\
    CHAT_ROOM_ENTER_RESPONSE_MEMBER_BLOCKED_YOU = 10,								\
    CHAT_ROOM_ENTER_RESPONSE_YOU_BLOCKED_MEMBER = 11								\
};																					\
enum ChatSteamIDInstanceFlags {														\
    CHAT_ACCOUNT_INSTANCE_MASK = 0X00000FFF,										\
    CHAT_INSTANCE_FLAG_CLAN = ((k_unSteamAccountInstanceMask + 1) >> 1),			\
    CHAT_INSTANCE_FLAG_LOBBY = ((k_unSteamAccountInstanceMask + 1) >> 2),			\
    CHAT_INSTANCE_FLAG_MMS_LOBBY = ((k_unSteamAccountInstanceMask + 1) >> 3)		\
};																					\
enum DenyReason {																	\
    DENY_INVALID = 0,																\
    DENY_INVALID_VERSION = 1,														\
    DENY_GENERIC = 2,																\
    DENY_NOT_LOGGED_ON = 3,															\
    DENY_NO_LICENSE = 4,															\
    DENY_CHEATER = 5,																\
    DENY_LOGGED_IN_ELSEWHERE = 6,													\
    DENY_UNKNOWN_TEXT = 7,															\
    DENY_INCOMPATIBLE_ANTI_CHEAT = 8,												\
    DENY_MEMORY_CORRUPTION = 9,														\
    DENY_INCOMPATIBLE_SOFTWARE = 10,												\
    DENY_STEAM_CONNECTION_LOST = 11,												\
    DENY_STEAM_CONNECTION_ERROR = 12,												\
    DENY_STEAM_RESPONSE_TIMED_OUT = 13,												\
    DENY_STEAM_VALIDATION_STALLED = 14,												\
    DENY_STEAM_OWNER_LEFT_GUEST_USER = 15											\
};																					\
enum GameIDType {																	\
    GAME_TYPE_APP = 0,																\
    GAME_TYPE_GAME_MOD = 1,															\
    GAME_TYPE_SHORTCUT = 2,															\
    GAME_TYPE_P2P = 3																\
};																					\
enum LaunchOptionType {																\
    LAUNCH_OPTION_TYPE_NONE = 0,													\
    LAUNCH_OPTION_TYPE_DEFAULT = 1,													\
    LAUNCH_OPTION_TYPE_SAFE_MODE = 2,												\
    LAUNCH_OPTION_TYPE_MULTIPLAYER = 3,												\
    LAUNCH_OPTION_TYPE_CONFIG = 4,													\
    LAUNCH_OPTION_TYPE_OPEN_VR = 5,													\
    LAUNCH_OPTION_TYPE_SERVER = 6,													\
    LAUNCH_OPTION_TYPE_EDITOR = 7,													\
    LAUNCH_OPTION_TYPE_MANUAL = 8,													\
    LAUNCH_OPTION_TYPE_BENCHMARK = 9,												\
    LAUNCH_OPTION_TYPE_OPTION1 = 10,												\
    LAUNCH_OPTION_TYPE_OPTION2 = 11,												\
    LAUNCH_OPTION_TYPE_OPTION3 = 12,												\
    LAUNCH_OPTION_TYPE_OCULUS_VR = 13,												\
    LAUNCH_OPTION_TYPE_OPEN_VR_OVERLAY = 14,										\
    LAUNCH_OPTION_TYPE_OS_VR = 15,													\
    LAUNCH_OPTION_TYPE_DIALOG = 1000												\
};																					\
enum NotificationPosition {															\
    POSITION_TOP_LEFT = 0,															\
    POSITION_TOP_RIGHT = 1,															\
    POSITION_BOTTOM_LEFT = 2,														\
    POSITION_BOTTOM_RIGHT = 3														\
};																					\
enum Result {																		\
    RESULT_OK = 1,																	\
    RESULT_FAIL = 2,																\
    RESULT_NO_CONNECTION = 3,														\
    RESULT_INVALID_PASSWORD = 5,													\
    RESULT_LOGGED_IN_ELSEWHERE = 6,													\
    RESULT_INVALID_PROTOCOL_VER = 7,												\
    RESULT_INVALID_PARAM = 8,														\
    RESULT_FILE_NOT_FOUND = 9,														\
    RESULT_BUSY = 10,																\
    RESULT_INVALID_STATE = 11,														\
    RESULT_INVALID_NAME = 12,														\
    RESULT_INVALID_EMAIL = 13,														\
    RESULT_DUPLICATE_NAME = 14,														\
    RESULT_ACCESS_DENIED = 15,														\
    RESULT_TIMEOUT = 16,															\
    RESULT_BANNED = 17,																\
    RESULT_ACCOUNT_NOT_FOUND = 18,													\
    RESULT_INVALID_STEAM_ID = 19,													\
    RESULT_SERVICE_UNAVAILABLE = 20,												\
    RESULT_NOT_LOGGED_ON = 21,														\
    RESULT_PENDING = 22,															\
    RESULT_ENCRYPTION_FAILURE = 23,													\
    RESULT_INSUFFICIENT_PRIVILEGE = 24,												\
    RESULT_LIMIT_EXCEEDED = 25,														\
    RESULT_REVOKED = 26,															\
    RESULT_EXPIRED = 27,															\
    RESULT_ALREADY_REDEEMED = 28,													\
    RESULT_DUPLICATE_REQUEST = 29,													\
    RESULT_ALREADY_OWNED = 30,														\
    RESULT_IP_NOT_FOUND = 31,														\
    RESULT_PERSIST_FAILED = 32,														\
    RESULT_LOCKING_FAILED = 33,														\
    RESULT_LOG_ON_SESSION_REPLACED = 34,											\
    RESULT_CONNECT_FAILED = 35,														\
    RESULT_HANDSHAKE_FAILED = 36,													\
    RESULT_IO_FAILURE = 37,															\
    RESULT_REMOTE_DISCONNECT = 38,													\
    RESULT_SHOPPING_CART_NOT_FOUND = 39,											\
    RESULT_BLOCKED = 40,															\
    RESULT_IGNORED = 41,															\
    RESULT_NO_MATCH = 42,															\
    RESULT_ACCOUNT_DISABLED = 43,													\
    RESULT_SERVICE_READ_ONLY = 44,													\
    RESULT_ACCOUNT_NOT_FEATURED = 45,												\
    RESULT_ADMINISTRATOR_OK = 46,													\
    RESULT_CONTENT_VERSION = 47,													\
    RESULT_TRY_ANOTHER_CM = 48,														\
    RESULT_PASSWORD_REQUIRED_TO_KICK_SESSION = 49,									\
    RESULT_ALREADY_LOGGED_IN_ELSEWHERE = 50,										\
    RESULT_SUSPENDED = 51,															\
    RESULT_CANCELLED = 52,															\
    RESULT_DATA_CORRUPTION = 53,													\
    RESULT_DISK_FULL = 54,															\
    RESULT_REMOTE_CALL_FAILED = 55,													\
    RESULT_PASSWORD_UNSET = 56,														\
    RESULT_EXTERNAL_ACCOUNT_UNLINKED = 57,											\
    RESULT_PSN_TICKET_INVALID = 58,													\
    RESULT_EXTERNAL_ACCOUNT_ALREADY_LINKED = 59,									\
    RESULT_REMOTE_FILE_CONFLICT = 60,												\
    RESULT_ILLEGAL_PASSWORD = 61,													\
    RESULT_SAME_AS_PREVIOUS_VALUE = 62,												\
    RESULT_ACCOUNT_LOG_ON_DENIED = 63,												\
    RESULT_CANNOT_USE_OLD_PASSWORD = 64,											\
    RESULT_INVALID_LOGIN_AUTH_CODE = 65,											\
    RESULT_ACCOUNT_LOG_ON_DENIED_NO_MAIL = 66,										\
    RESULT_HARDWARE_NOT_CAPABLE_OF_IPT = 67,										\
    RESULT_IPT_INIT_ERROR = 68,														\
    RESULT_PARENTAL_CONTROL_RESTRICTED = 69,										\
    RESULT_FACEBOOK_QUERY_ERROR = 70,												\
    RESULT_EXPIRED_LOGIN_AUTH_CODE = 71,											\
    RESULT_IP_LOGIN_RESTRICTION_FAILED = 72,										\
    RESULT_ACCOUNT_LOCKED_DOWN = 73,												\
    RESULT_ACCOUNT_LOG_ON_DENIED_VERIFIED_EMAIL_REQUIRED = 74,						\
    RESULT_NO_MATCHING_URL = 75,													\
    RESULT_BAD_RESPONSE = 76,														\
    RESULT_REQUIRE_PASSWORD_REENTRY = 77,											\
    RESULT_VALUE_OUT_OF_RANGE = 78,													\
    RESULT_UNEXPECTED_ERROR = 79,													\
    RESULT_DISABLED = 80,															\
    RESULT_INVALID_CEG_SUBMISSION = 81,												\
    RESULT_RESTRICTED_DEVICE = 82,													\
    RESULT_REGION_LOCKED = 83,														\
    RESULT_RATE_LIMIT_EXCEEDED = 84,												\
    RESULT_ACCOUNT_LOGIN_DENIED_NEED_TWO_FACTOR = 85,								\
    RESULT_ITEM_DELETED = 86,														\
    RESULT_ACCOUNT_LOGIN_DENIED_THROTTLE = 87,										\
    RESULT_TWO_FACTOR_CODE_MISMATCH = 88,											\
    RESULT_TWO_FACTOR_ACTIVATION_CODE_MISMATCH = 89,								\
    RESULT_ACCOUNT_ASSOCIATED_TO_MULTIPLE_PARTNERS = 90,							\
    RESULT_NOT_MODIFIED = 91,														\
    RESULT_NO_MOBILE_DEVICE = 92,													\
    RESULT_TIME_NOT_SYNCED = 93,													\
    RESULT_SMS_CODE_FAILED = 94,													\
    RESULT_ACCOUNT_LIMIT_EXCEEDED = 95,												\
    RESULT_ACCOUNT_ACTIVITY_LIMIT_EXCEEDED = 96,									\
    RESULT_PHONE_ACTIVITY_LIMIT_EXCEEDED = 97,										\
    RESULT_REFUND_TO_WALLET = 98,													\
    RESULT_EMAIL_SEND_FAILURE = 99,													\
    RESULT_NOT_SETTLED = 100,														\
    RESULT_NEED_CAPTCHA = 101,														\
    RESULT_GSLT_DENIED = 102,														\
    RESULT_GS_OWNER_DENIED = 103,													\
    RESULT_INVALID_ITEM_TYPE = 104,													\
    RESULT_IP_BANNED = 105,															\
    RESULT_GSLT_EXPIRED = 106,														\
    RESULT_INSUFFICIENT_FUNDS = 107,												\
    RESULT_TOO_MANY_PENDING = 108													\
};																					\
enum Universe {																		\
    UNIVERSE_INVALID = 0,															\
    UNIVERSE_PUBLIC = 1,															\
    UNIVERSE_BETA = 2,																\
    UNIVERSE_INTERNAL = 3,															\
    UNIVERSE_DEV = 4,																\
    UNIVERSE_MAX = 5																\
};																					\
enum UserHasLicenseForAppResult {													\
    USER_HAS_LICENSE_RESULT_HAS_LICENSE = 0,										\
    USER_HAS_LICENSE_RESULT_DOES_NOT_HAVE_LICENSE = 1,								\
    USER_HAS_LICENSE_RESULT_NO_AUTH = 2												\
};																					\
enum VoiceResult {																	\
    VOICE_RESULT_OK = 0,															\
    VOICE_RESULT_NOT_INITIALIZED = 1,												\
    VOICE_RESULT_NOT_RECORDING = 2,													\
    VOICE_RESULT_NO_DATE = 3,														\
    VOICE_RESULT_BUFFER_TOO_SMALL = 4,												\
    VOICE_RESULT_DATA_CORRUPTED = 5,												\
    VOICE_RESULT_RESTRICTED = 6														\
};																					\
enum VRHMDType {																	\
    VR_HMD_TYPE_NONE = -1,															\
    VR_HMD_TYPE_UNKNOWN = 0,														\
    VR_HMD_TYPE_HTC_DEV = 1,														\
    VR_HMD_TYPE_HTC_VIVEPRE = 2,													\
    VR_HMD_TYPE_HTC_VIVE = 3,														\
    VR_HMD_TYPE_HTC_UNKNOWN = 20,													\
    VR_HMD_TYPE_OCULUS_DK1 = 21,													\
    VR_HMD_TYPE_OCULUS_DK2 = 22,													\
    VR_HMD_TYPE_OCULUS_RIFT = 23,													\
    VR_HMD_TYPE_OCULUS_UNKNOWN = 40													\
};																					\
																					\
/* Friends enums */																	\
enum AvatarSizes {																	\
    AVATAR_SMALL = 1,																\
    AVATAR_MEDIUM = 2,																\
    AVATAR_LARGE = 3																\
};																					\
enum FriendFlags {																	\
    FRIEND_FLAG_NONE = 0X00,														\
    FRIEND_FLAG_BLOCKED = 0X01,														\
    FRIEND_FLAG_FRIENDSHIP_REQUESTED = 0X02,										\
    FRIEND_FLAG_IMMEDIATE = 0X04,													\
    FRIEND_FLAG_CLAN_MEMBER = 0X08,													\
    FRIEND_FLAG_ON_GAME_SERVER = 0X10,												\
    FRIEND_FLAG_REQUESTING_FRIENDSHIP = 0X80,										\
    FRIEND_FLAG_REQUESTING_INFO = 0X100,											\
    FRIEND_FLAG_IGNORED = 0X200,													\
    FRIEND_FLAG_IGNORED_FRIEND = 0X400,												\
    FRIEND_FLAG_CHAT_MEMBER = 0X1000,												\
    FRIEND_FLAG_ALL = 0XFFFF														\
};																					\
enum FriendRelationship {															\
    FRIEND_RELATION_NONE = 0,														\
    FRIEND_RELATION_BLOCKED = 1,													\
    FRIEND_RELATION_REQUEST_RECIPIENT = 2,											\
    FRIEND_RELATION_FRIEND = 3,														\
    FRIEND_RELATION_REQUEST_INITIATOR = 4,FRIEND_RELATION_IGNORED = 5,				\
    FRIEND_RELATION_IGNORED_FRIEND = 6,												\
    FRIEND_RELATION_SUGGESTED = 7,													\
    FRIEND_RELATION_MAX = 8															\
};																					\
enum OverlayToStoreFlag {															\
    OVERLAY_TO_STORE_FLAG_NONE = 0,													\
    OVERLAY_TO_STORE_FLAG_ADD_TO_CART = 1,											\
    OVERLAY_TO_STORE_FLAG_AND_TO_CART_AND_SHOW = 2									\
};																					\
enum OverlayToWebPageMode {															\
    OVERLAY_TO_WEB_PAGE_MODE_DEFAULT = 0,											\
    OVERLAY_TO_WEB_PAGE_MODE_MODAL = 1												\
};																					\
enum PersonaChange {																\
    PERSONA_CHANGE_NAME = 0X0001,													\
    PERSONA_CHANGE_STATUS = 0X0002,													\
    PERSONA_CHANGE_COME_ONLINE = 0X0004,											\
    PERSONA_CHANGE_GONE_OFFLINE = 0X0008,											\
    PERSONA_CHANGE_GAME_PLAYED = 0X0010,											\
    PERSONA_CHANGE_GAME_SERVER = 0X0020,											\
    PERSONA_CHANGE_AVATAR = 0X0040,													\
    PERSONA_CHANGE_JOINED_SOURCE = 0X0080,											\
    PERSONA_CHANGE_LEFT_SOURCE = 0X0100,											\
    PERSONA_CHANGE_RELATIONSHIP_CHANGED = 0X0200,									\
    PERSONA_CHANGE_NAME_FIRST_SET = 0X0400,											\
    PERSONA_CHANGE_FACEBOOK_INFO = 0X0800,											\
    PERSONA_CHANGE_NICKNAME = 0X1000,												\
    PERSONA_CHANGE_STEAM_LEVEL = 0X2000												\
};																					\
enum PersonaState {																	\
    PERSONA_STATE_OFFLINE = 0,														\
    PERSONA_STATE_ONLINE = 1,														\
    PERSONA_STATE_BUSY = 2,															\
    PERSONA_STATE_AWAY = 3,															\
    PERSONA_STATE_SNOOZE = 4,														\
    PERSONA_STATE_LOOKING_TO_TRADE = 5,												\
    PERSONA_STATE_LOOKING_TO_PLAY = 6,												\
    PERSONA_STATE_MAX = 7															\
};																					\
enum UserRestriction {																\
    USER_RESTRICTION_NONE = 0,														\
    USER_RESTRICTION_UNKNOWN = 1,													\
    USER_RESTRICTION_ANY_CHAT = 2,													\
    USER_RESTRICTION_VOICE_CHAT = 4,												\
    USER_RESTRICTION_GROUP_CHAT = 8,												\
    USER_RESTRICTION_RATING = 16,													\
    USER_RESTRICTION_GAME_INVITES = 32,												\
    USER_RESTRICTION_TRADING = 64													\
};																					\
enum CommunityProfileItemType {														\
    PROFILE_ITEM_TYPE_ANIMATED_AVATAR = 0,											\
    PROFILE_ITEM_TYPE_AVATAR_FRAME = 1,												\
    PROFILE_ITEM_TYPE_PROFILE_MODIFIER = 2,											\
    PROFILE_ITEM_TYPE_PROFILE_BACKGROUND = 3,										\
    PROFILE_ITEM_TYPE_MINI_PROFILE_BACKGROUND = 4									\
};																					\
enum CommunityProfileItemProperty {													\
    PROFILE_ITEM_PROPERTY_IMAGE_SMALL = 0,											\
    PROFILE_ITEM_PROPERTY_IMAGE_LARGE = 1,											\
    PROFILE_ITEM_PROPERTY_INTERNAL_NAME = 2,										\
    PROFILE_ITEM_PROPERTY_TITLE = 3,												\
    PROFILE_ITEM_PROPERTY_DESCRIPTION = 4,											\
    PROFILE_ITEM_PROPERTY_APP_ID = 5,												\
    PROFILE_ITEM_PROPERTY_TYPE_ID = 6,												\
    PROFILE_ITEM_PROPERTY_CLASS = 7,												\
    PROFILE_ITEM_PROPERTY_MOVIE_WEBM = 8,											\
    PROFILE_ITEM_PROPERTY_MOVIE_MP4 = 9,											\
    PROFILE_ITEM_PROPERTY_MOVIE_WEBM_SMALL = 10,									\
    PROFILE_ITEM_PROPERTY_MOVIE_MP4_SMALL = 11										\
};																					\
																					\
/* Game Search enums */																\
enum GameSearchErrorCode {															\
    GAME_SEARCH_ERROR_CODE_OK = 1,													\
    GAME_SEARCH_ERROR_CODE_SEARCH_AREADY_IN_PROGRESS = 2,							\
    GAME_SEARCH_ERROR_CODE_NO_SEARCH_IN_PROGRESS = 3,								\
    GAME_SEARCH_ERROR_CODE_NOT_LOBBY_LEADER = 4,									\
    GAME_SEARCH_ERROR_CODE_NO_HOST_AVAILABLE = 5,									\
    GAME_SEARCH_ERROR_CODE_SEARCH_PARAMS_INVALID = 6,								\
    GAME_SEARCH_ERROR_CODE_OFFLINE = 7,												\
    GAME_SEARCH_ERROR_CODE_NOT_AUTHORIZED = 8,										\
    GAME_SEARCH_ERROR_CODE_UNKNOWN_ERROR = 9										\
};																					\
enum PlayerResult {																	\
    PLAYER_RESULT_FAILED_TO_CONNECT = 1,											\
    PLAYER_RESULT_ABANDONED = 2,													\
    PLAYER_RESULT_KICKED = 3,														\
    PLAYER_RESULT_INCOMPLETE = 4,													\
    PLAYER_RESULT_COMPLETED = 5														\
};																					\
																					\
/* HTMLSurface enums */																\
enum HTMLKeyModifiers {																\
    HTML_KEY_MODIFIER_NONE = 0,														\
    HTML_KEY_MODIFIER_ALT_DOWN = (1<<0),											\
    HTML_KEY_MODIFIER_CTRL_DOWN = (1<<1),											\
    HTML_KEY_MODIFIER_SHIFT_DOWN = (1<<2)											\
};																					\
enum HTMLMouseButton {																\
    HTML_MOUSE_BUTTON_LEFT = 0,														\
    HTML_MOUSE_BUTTON_RIGHT = 1,													\
    HTML_MOUSE_BUTTON_MIDDLE = 2													\
};																					\
enum MouseCursor {																	\
    DC_USER = 0,																	\
    DC_NONE = 1,																	\
    DC_ARROW = 2,																	\
    DC_IBEAM = 3,																	\
    DC_HOUR_GLASS = 4,																\
    DC_WAIT_ARROW = 5,																\
    DC_CROSSHAIR = 6,																\
    DC_UP = 7,																		\
    DC_SIZE_NW = 8,																	\
    DC_SIZE_SE = 9,																	\
    DC_SIZE_NE = 10,																\
    DC_SIZE_SW = 11,																\
    DC_SIZE_W = 12,																	\
    DC_SIZE_E = 13,																	\
    DC_SIZE_N = 14,																	\
    DC_SIZE_S = 15,																	\
    DC_SIZE_WE = 16,																\
    DC_SIZE_NS = 17,																\
    DC_SIZE_ALL = 18,																\
    DC_NO = 19,																		\
    DC_HAND = 20,																	\
    DC_BLANK = 21,																	\
    DC_MIDDLE_PAN = 22,																\
    DC_NORTH_PAN = 23,																\
    DC_NORTH_EAST_PAN = 24,															\
    DC_EAST_PAN = 25,																\
    DC_SOUTH_EAST_PAN = 26,															\
    DC_SOUTH_PAN = 27,																\
    DC_SOUTH_WEST_PAN = 28,															\
    DC_WEST_PAN = 29,																\
    DC_NORTH_WEST_PAN = 30,															\
    DC_ALIAS = 31,																	\
    DC_CELL = 32,																	\
    DC_COL_RESIZE = 33,																\
    DC_COPY_CUR = 34,																\
    DC_VERTICAL_TEXT = 35,															\
    DC_ROW_RESIZE = 36,																\
    DC_ZOOM_IN = 37,																\
    DC_ZOOM_OUT = 38,																\
    DC_HELP = 39,																	\
    DC_CUSTOM = 40,																	\
    DC_LAST = 41																	\
};																					\
																					\
/* HTTP enums */																	\
enum HTTPMethod {																	\
    HTTP_METHOD_INVALID = 0,														\
    HTTP_METHOD_GET = 1,															\
    HTTP_METHOD_HEAD = 2,															\
    HTTP_METHOD_POST = 3,															\
    HTTP_METHOD_PUT = 4,															\
    HTTP_METHOD_DELETE = 5,															\
    HTTP_METHOD_OPTIONS = 6,														\
    HTTP_METHOD_PATCH = 7															\
};																					\
enum HTTPStatusCode {																\
    HTTP_STATUS_CODE_INVALID = 0,													\
    HTTP_STATUS_CODE_100_CONTINUE = 100,											\
    HTTP_STATUS_CODE_101_SWITCHING_PROTOCOLS = 101,									\
    HTTP_STATUS_CODE_200_OK = 200,													\
    HTTP_STATUS_CODE_201_CREATED = 201,												\
    HTTP_STATUS_CODE_202_ACCEPTED = 202,											\
    HTTP_STATUS_CODE_203_NON_AUTHORITATIVE = 203,									\
    HTTP_STATUS_CODE_204_NO_CONTENT = 204,											\
    HTTP_STATUS_CODE_205_RESET_CONTENT = 205,										\
    HTTP_STATUS_CODE_206_PARTIAL_CONTENT = 206,										\
    HTTP_STATUS_CODE_300_MULTIPLE_CHOICES = 300,									\
    HTTP_STATUS_CODE_301_MOVED_PERMANENTLY = 301,									\
    HTTP_STATUS_CODE_302_FOUND = 302,												\
    HTTP_STATUS_CODE_303_SEE_OTHER = 303,											\
    HTTP_STATUS_CODE_304_NOT_MODIFIED = 304,										\
    HTTP_STATUS_CODE_305_USE_PROXY = 305,											\
    HTTP_STATUS_CODE_307_TEMPORARY_REDIRECT = 307,									\
    HTTP_STATUS_CODE_400_BAD_REQUEST = 400,											\
    HTTP_STATUS_CODE_401_UNAUTHORIZED = 401,										\
    HTTP_STATUS_CODE_402_PAYMENT_REQUIRED = 402,									\
    HTTP_STATUS_CODE_403_FORBIDDEN = 403,											\
    HTTP_STATUS_CODE_404_NOT_FOUND = 404,											\
    HTTP_STATUS_CODE_405_METHOD_NOT_ALLOWED = 405,									\
    HTTP_STATUS_CODE_406_NOT_ACCEPTABLE = 406,										\
    HTTP_STATUS_CODE_407_PROXY_AUTH_REQUIRED = 407,									\
    HTTP_STATUS_CODE_408_REQUEST_TIMEOUT = 408,										\
    HTTP_STATUS_CODE_409_CONFLICT = 409,											\
    HTTP_STATUS_CODE_410_GONE = 410,												\
    HTTP_STATUS_CODE_411_LENGTH_REQUIRED = 411,										\
    HTTP_STATUS_CODE_412_PRECONDITION_FAILED = 412,									\
    HTTP_STATUS_CODE_413_REQUEST_ENTITY_TOO_LARGE = 413,							\
    HTTP_STATUS_CODE_414_REQUEST_URI_TOO_LONG = 414,								\
    HTTP_STATUS_CODE_415_UNSUPPORTED_MEDIA_TYPE = 415,								\
    HTTP_STATUS_CODE_416_REQUESTED_RANGE_NOT_SATISFIABLE = 416,						\
    HTTP_STATUS_CODE_417_EXPECTATION_FAILED = 417,									\
    HTTP_STATUS_CODE_4XX_UNKNOWN = 418,												\
    HTTP_STATUS_CODE_429_TOO_MANY_REQUESTS = 429,									\
    HTTP_STATUS_CODE_500_INTERNAL_SERVER_ERROR = 500,								\
    HTTP_STATUS_CODE_501_NOT_IMPLEMENTED = 501,										\
    HTTP_STATUS_CODE_502_BAD_GATEWAY = 502,											\
    HTTP_STATUS_CODE_503_SERVICE_UNAVAILABLE = 503,									\
    HTTP_STATUS_CODE_504_GATEWAY_TIMEOUT = 504,										\
    HTTP_STATUS_CODE_505_HTTP_VERSION_NOT_SUPPORTED = 505,							\
    HTTP_STATUS_CODE_5XX_UNKNOWN = 599												\
};																					\
																					\
/* Input enums */																		\
enum InputActionOrigin {															\
    INPUT_ACTION_ORIGIN_NONE = 0,													\
	INPUT_ACTION_ORIGIN_A = 1,														\
	INPUT_ACTION_ORIGIN_B = 2,														\
	INPUT_ACTION_ORIGIN_X = 3,														\
	INPUT_ACTION_ORIGIN_Y = 4,														\
	INPUT_ACTION_ORIGIN_LEFT_BUMPER = 5,											\
	INPUT_ACTION_ORIGIN_RIGHT_BUMPER = 6,											\
	INPUT_ACTION_ORIGIN_LEFTGRIP = 7,												\
	INPUT_ACTION_ORIGIN_RIGHTGRIP = 8,												\
	INPUT_ACTION_ORIGIN_START = 9,													\
	INPUT_ACTION_ORIGIN_BACK = 10,													\
																					\
    INPUT_ACTION_ORIGIN_LEFT_PAD_TOUCH = 11,										\
	INPUT_ACTION_ORIGIN_LEFT_PAD_SWIPE = 12,										\
	INPUT_ACTION_ORIGIN_LEFT_PAD_CLICK = 13,										\
	INPUT_ACTION_ORIGIN_LEFT_PAD_DPAD_NORTH = 14,									\
	INPUT_ACTION_ORIGIN_LEFT_PAD_DPAD_SOUTH = 15,									\
	INPUT_ACTION_ORIGIN_LEFT_PAD_DPAD_WEST = 16,									\
	INPUT_ACTION_ORIGIN_LEFT_PAD_DPAD_EAST = 17,									\
	INPUT_ACTION_ORIGIN_RIGHT_PAD_TOUCH = 18,										\
																					\
    INPUT_ACTION_ORIGIN_RIGHT_PAD_SWIPE = 19,										\
	INPUT_ACTION_ORIGIN_RIGHT_PAD_CLICK = 20,										\
	INPUT_ACTION_ORIGIN_RIGHT_PAD_DPAD_NORTH = 21,									\
	INPUT_ACTION_ORIGIN_RIGHT_PAD_DPAD_SOUTH = 22,									\
	INPUT_ACTION_ORIGIN_RIGHT_PAD_DPAD_WEST = 23,									\
	INPUT_ACTION_ORIGIN_RIGHT_PAD_DPAD_EAST = 24,									\
	INPUT_ACTION_ORIGIN_LEFT_TRIGGER_PULL = 25,										\
	INPUT_ACTION_ORIGIN_LEFT_TRIGGER_CLICK = 26,									\
																					\
    INPUT_ACTION_ORIGIN_RIGHT_TRIGGER_PULL = 27,									\
	INPUT_ACTION_ORIGIN_RIGHT_TRIGGER_CLICK = 28,									\
	INPUT_ACTION_ORIGIN_LEFT_STICK_MOVE = 29,										\
	INPUT_ACTION_ORIGIN_LEFT_STICK_CLICK = 30,										\
	INPUT_ACTION_ORIGIN_LEFT_STICK_DPAD_NORTH = 31,									\
	INPUT_ACTION_ORIGIN_LEFT_STICK_DPAD_SOUTH = 32,									\
	INPUT_ACTION_ORIGIN_LEFT_STICK_DPAD_WEST = 33,									\
																					\
    INPUT_ACTION_ORIGIN_LEFT_STICK_DPAD_EAST = 34,									\
	INPUT_ACTION_ORIGIN_GYRO_MOVE = 35,												\
	INPUT_ACTION_ORIGIN_GYRO_PITCH = 36,											\
	INPUT_ACTION_ORIGIN_GYRO_YAW = 37,												\
	INPUT_ACTION_ORIGIN_GYRO_ROLL = 38,												\
	INPUT_ACTION_ORIGIN_STEAM_CONTROLLER_RESERVED0 = 39,							\
	INPUT_ACTION_ORIGIN_STEAM_CONTROLLER_RESERVED1 = 40,							\
	INPUT_ACTION_ORIGIN_STEAM_CONTROLLER_RESERVED2 = 41,							\
																					\
    INPUT_ACTION_ORIGIN_STEAM_CONTROLLER_RESERVED3 = 42,							\
	INPUT_ACTION_ORIGIN_STEAM_CONTROLLER_RESERVED4 = 43,							\
	INPUT_ACTION_ORIGIN_STEAM_CONTROLLER_RESERVED5 = 44,							\
	INPUT_ACTION_ORIGIN_STEAM_CONTROLLER_RESERVED6 = 45,							\
	INPUT_ACTION_ORIGIN_STEAM_CONTROLLER_RESERVED7 = 46,							\
	INPUT_ACTION_ORIGIN_STEAM_CONTROLLER_RESERVED8 = 47,							\
																					\
    INPUT_ACTION_ORIGIN_STEAM_CONTROLLER_RESERVED9 = 48,							\
	INPUT_ACTION_ORIGIN_STEAM_CONTROLLER_RESERVED10 = 49,							\
	INPUT_ACTION_ORIGIN_PS4_X = 50,													\
	INPUT_ACTION_ORIGIN_PS4_CIRCLE = 51,											\
	INPUT_ACTION_ORIGIN_PS4_TRIANGLE = 52,											\
	INPUT_ACTION_ORIGIN_PS4_SQUARE = 53,											\
	INPUT_ACTION_ORIGIN_PS4_LEFT_BUMPER = 54,										\
	INPUT_ACTION_ORIGIN_PS4_RIGHT_BUMPER = 55,										\
																					\
    INPUT_ACTION_ORIGIN_PS4_OPTIONS = 56,											\
	INPUT_ACTION_ORIGIN_PS4_SHARE = 57,												\
	INPUT_ACTION_ORIGIN_PS4_LEFT_PAD_TOUCH = 58,									\
	INPUT_ACTION_ORIGIN_PS4_LEFT_PAD_SWIPE = 59,									\
	INPUT_ACTION_ORIGIN_PS4_LEFT_PAD_CLICK = 60,									\
	INPUT_ACTION_ORIGIN_PS4_LEFT_PAD_DPAD_NORTH = 61,								\
	INPUT_ACTION_ORIGIN_PS4_LEFT_PAD_DPAD_SOUTH = 62,								\
	INPUT_ACTION_ORIGIN_PS4_LEFT_PAD_DPAD_WEST = 63,								\
																					\
    INPUT_ACTION_ORIGIN_PS4_LEFT_PAD_DPAD_EAST = 64,								\
	INPUT_ACTION_ORIGIN_PS4_RIGHT_PAD_TOUCH = 65,									\
	INPUT_ACTION_ORIGIN_PS4_RIGHT_PAD_SWIPE = 66,									\
	INPUT_ACTION_ORIGIN_PS4_RIGHT_PAD_CLICK = 67,									\
	INPUT_ACTION_ORIGIN_PS4_RIGHT_PAD_DPAD_NORTH = 68,								\
	INPUT_ACTION_ORIGIN_PS4_RIGHT_PAD_DPAD_SOUTH = 69,								\
	INPUT_ACTION_ORIGIN_PS4_RIGHT_PAD_DPAD_WEST = 70,								\
																					\
    INPUT_ACTION_ORIGIN_PS4_RIGHT_PAD_DPAD_EAST = 71,								\
	INPUT_ACTION_ORIGIN_PS4_CENTER_PAD_TOUCH = 72,									\
	INPUT_ACTION_ORIGIN_PS4_CENTER_PAD_SWIPE = 73,									\
	INPUT_ACTION_ORIGIN_PS4_CENTER_PAD_CLICK = 74,									\
	INPUT_ACTION_ORIGIN_PS4_CENTER_PAD_DPAD_NORTH = 75,								\
	INPUT_ACTION_ORIGIN_PS4_CENTER_PAD_DPAD_SOUTH = 76,								\
	INPUT_ACTION_ORIGIN_PS4_CENTER_PAD_DPAD_WEST = 77,								\
																					\
    INPUT_ACTION_ORIGIN_PS4_CENTER_PAD_DPAD_EAST = 78,								\
	INPUT_ACTION_ORIGIN_PS4_LEFT_TRIGGER_PULL = 79,									\
	INPUT_ACTION_ORIGIN_PS4_LEFT_TRIGGER_CLICK = 80,								\
	INPUT_ACTION_ORIGIN_PS4_RIGHT_TRIGGER_PULL = 81,								\
	INPUT_ACTION_ORIGIN_PS4_RIGHT_TRIGGER_CLICK = 82,								\
	INPUT_ACTION_ORIGIN_PS4_LEFT_STICK_MOVE = 83,									\
	INPUT_ACTION_ORIGIN_PS4_LEFT_STICK_CLICK = 84,									\
																					\
    INPUT_ACTION_ORIGIN_PS4_LEFT_STICK_DPAD_NORTH = 85,								\
	INPUT_ACTION_ORIGIN_PS4_LEFT_STICK_DPAD_SOUTH = 86,								\
	INPUT_ACTION_ORIGIN_PS4_LEFT_STICK_DPAD_WEST = 87,								\
	INPUT_ACTION_ORIGIN_PS4_LEFT_STICK_DPAD_EAST = 88,								\
	INPUT_ACTION_ORIGIN_PS4_RIGHT_STICK_MOVE = 89,									\
	INPUT_ACTION_ORIGIN_PS4_RIGHT_STICK_CLICK = 90,									\
	INPUT_ACTION_ORIGIN_PS4_RIGHT_STICK_DPAD_NORTH = 91,							\
																					\
    INPUT_ACTION_ORIGIN_PS4_RIGHT_STICK_DPAD_SOUTH = 92,							\
	INPUT_ACTION_ORIGIN_PS4_RIGHT_STICK_DPAD_WEST = 93,								\
	INPUT_ACTION_ORIGIN_PS4_RIGHT_STICK_DPAD_EAST = 94,								\
	INPUT_ACTION_ORIGIN_PS4_DPAD_NORTH = 95,										\
	INPUT_ACTION_ORIGIN_PS4_DPAD_SOUTH = 96,										\
	INPUT_ACTION_ORIGIN_PS4_DPAD_WEST = 97,											\
	INPUT_ACTION_ORIGIN_PS4_DPAD_EAST = 98,											\
	INPUT_ACTION_ORIGIN_PS4_GYRO_MOVE = 99,											\
																					\
    INPUT_ACTION_ORIGIN_PS4_GYRO_PITCH = 100,										\
	INPUT_ACTION_ORIGIN_PS4_GYRO_YAW = 101,											\
	INPUT_ACTION_ORIGIN_PS4_GYRO_ROLL = 102,										\
	INPUT_ACTION_ORIGIN_PS4_RESERVED0 = 103,										\
	INPUT_ACTION_ORIGIN_PS4_RESERVED1 = 104,										\
	INPUT_ACTION_ORIGIN_PS4_RESERVED2 = 105,										\
	INPUT_ACTION_ORIGIN_PS4_RESERVED3 = 106,										\
	INPUT_ACTION_ORIGIN_PS4_RESERVED4 = 107,										\
    INPUT_ACTION_ORIGIN_PS4_RESERVED5 = 108,										\
	INPUT_ACTION_ORIGIN_PS4_RESERVED6 = 109,										\
	INPUT_ACTION_ORIGIN_PS4_RESERVED7 = 110,										\
	INPUT_ACTION_ORIGIN_PS4_RESERVED8 = 111,										\
	INPUT_ACTION_ORIGIN_PS4_RESERVED9 = 112,										\
	INPUT_ACTION_ORIGIN_PS4_RESERVED10 = 113,										\
	INPUT_ACTION_ORIGIN_XBOX_ONE_A = 114,											\
	INPUT_ACTION_ORIGIN_XBOX_ONE_B = 115,											\
	INPUT_ACTION_ORIGIN_XBOX_ONE_X = 116,											\
																					\
    INPUT_ACTION_ORIGIN_XBOX_ONE_Y = 117,											\
	INPUT_ACTION_ORIGIN_XBOX_ONE_LEFT_BUMPER = 118,									\
	INPUT_ACTION_ORIGIN_XBOX_ONE_RIGHT_BUMPER = 119,								\
	INPUT_ACTION_ORIGIN_XBOX_ONE_MENU = 120,										\
	INPUT_ACTION_ORIGIN_XBOX_ONE_VIEW = 121,										\
	INPUT_ACTION_ORIGIN_XBOX_ONE_LEFT_TRIGGER_PULL = 122,							\
	INPUT_ACTION_ORIGIN_XBOX_ONE_LEFT_TRIGGER_CLICK = 123,							\
    INPUT_ACTION_ORIGIN_XBOX_ONE_RIGHT_TRIGGER_PULL = 124,							\
	INPUT_ACTION_ORIGIN_XBOX_ONE_RIGHT_TRIGGER_CLICK = 125,							\
	INPUT_ACTION_ORIGIN_XBOX_ONE_LEFT_STICK_MOVE = 126,								\
	INPUT_ACTION_ORIGIN_XBOX_ONE_LEFT_STICK_CLICK = 127,							\
	INPUT_ACTION_ORIGIN_XBOX_ONE_LEFT_STICK_DPAD_NORTH = 128,						\
	INPUT_ACTION_ORIGIN_XBOX_ONE_LEFT_STICK_DPAD_SOUTH = 129,						\
																					\
    INPUT_ACTION_ORIGIN_XBOX_ONE_LEFT_STICK_DPAD_WEST = 130,						\
	INPUT_ACTION_ORIGIN_XBOX_ONE_LEFT_STICK_DPAD_EAST = 131,						\
	INPUT_ACTION_ORIGIN_XBOX_ONE_RIGHT_STICK_MOVE = 132,							\
	INPUT_ACTION_ORIGIN_XBOX_ONE_RIGHT_STICK_CLICK = 133,							\
	INPUT_ACTION_ORIGIN_XBOX_ONE_RIGHT_STICK_DPAD_NORTH = 134,						\
	INPUT_ACTION_ORIGIN_XBOX_ONE_RIGHT_STICK_DPAD_SOUTH = 135,						\
    INPUT_ACTION_ORIGIN_XBOX_ONE_RIGHT_STICK_DPAD_WEST = 136,						\
	INPUT_ACTION_ORIGIN_XBOX_ONE_RIGHT_STICK_DPAD_EAST = 137,						\
	INPUT_ACTION_ORIGIN_XBOX_ONE_DPAD_NORTH = 138,									\
	INPUT_ACTION_ORIGIN_XBOX_ONE_DPAD_SOUTH = 139,									\
	INPUT_ACTION_ORIGIN_XBOX_ONE_DPAD_WEST = 140,									\
	INPUT_ACTION_ORIGIN_XBOX_ONE_DPAD_EAST = 141,									\
	INPUT_ACTION_ORIGIN_XBOX_ONE_RESERVED0 = 142,									\
																					\
    INPUT_ACTION_ORIGIN_XBOX_ONE_RESERVED1 = 143,									\
	INPUT_ACTION_ORIGIN_XBOX_ONE_RESERVED2 = 144,									\
	INPUT_ACTION_ORIGIN_XBOX_ONE_RESERVED3 = 145,									\
	INPUT_ACTION_ORIGIN_XBOX_ONE_RESERVED4 = 146,									\
	INPUT_ACTION_ORIGIN_XBOX_ONE_RESERVED5 = 147,									\
	INPUT_ACTION_ORIGIN_XBOX_ONE_RESERVED6 = 148,									\
	INPUT_ACTION_ORIGIN_XBOX_ONE_RESERVED7 = 149,									\
    INPUT_ACTION_ORIGIN_XBOX_ONE_RESERVED8 = 150,									\
	INPUT_ACTION_ORIGIN_XBOX_ONE_RESERVED9 = 151,									\
	INPUT_ACTION_ORIGIN_XBOX_ONE_RESERVED10 = 152,									\
	INPUT_ACTION_ORIGIN_XBOX_360_A = 153,											\
	INPUT_ACTION_ORIGIN_XBOX_360_B = 154,											\
	INPUT_ACTION_ORIGIN_XBOX_360_X = 155,											\
	INPUT_ACTION_ORIGIN_XBOX_360_Y = 156,											\
	INPUT_ACTION_ORIGIN_XBOX_360_LEFT_BUMPER = 157,									\
																					\
    INPUT_ACTION_ORIGIN_XBOX_360_RIGHT_BUMPER = 158,								\
	INPUT_ACTION_ORIGIN_XBOX_360_START = 159,										\
	INPUT_ACTION_ORIGIN_XBOX_360_BACK = 160,										\
	INPUT_ACTION_ORIGIN_XBOX_360_LEFT_TRIGGER_PULL = 161,							\
	INPUT_ACTION_ORIGIN_XBOX_360_LEFT_TRIGGER_CLICK = 162,							\
	INPUT_ACTION_ORIGIN_XBOX_360_RIGHT_TRIGGER_PULL = 163,							\
	INPUT_ACTION_ORIGIN_XBOX_360_RIGHT_TRIGGER_CLICK = 164,							\
																					\
    INPUT_ACTION_ORIGIN_XBOX_360_LEFT_STICK_MOVE = 165,								\
	INPUT_ACTION_ORIGIN_XBOX_360_LEFT_STICK_CLICK = 166,							\
	INPUT_ACTION_ORIGIN_XBOX_360_LEFT_STICK_DPAD_NORTH = 167,						\
	INPUT_ACTION_ORIGIN_XBOX_360_LEFT_STICK_DPAD_SOUTH = 168,						\
	INPUT_ACTION_ORIGIN_XBOX_360_LEFT_STICK_DPAD_WEST = 169,						\
	INPUT_ACTION_ORIGIN_XBOX_360_LEFT_STICK_DPAD_EAST = 170,						\
    INPUT_ACTION_ORIGIN_XBOX_360_RIGHT_STICK_MOVE = 171,							\
	INPUT_ACTION_ORIGIN_XBOX_360_RIGHT_STICK_CLICK = 172,							\
	INPUT_ACTION_ORIGIN_XBOX_360_RIGHT_STICK_DPAD_NORTH = 173,						\
	INPUT_ACTION_ORIGIN_XBOX_360_RIGHT_STICK_DPAD_SOUTH = 174,						\
	INPUT_ACTION_ORIGIN_XBOX_360_RIGHT_STICK_DPAD_WEST = 175,						\
	INPUT_ACTION_ORIGIN_XBOX_360_RIGHT_STICK_DPAD_EAST = 176,						\
																					\
    INPUT_ACTION_ORIGIN_XBOX_360_DPAD_NORTH = 177,									\
	INPUT_ACTION_ORIGIN_XBOX_360_DPAD_SOUTH = 178,									\
	INPUT_ACTION_ORIGIN_XBOX_360_DPAD_WEST = 179,									\
	INPUT_ACTION_ORIGIN_XBOX_360_DPAD_EAST = 180,									\
	INPUT_ACTION_ORIGIN_XBOX_360_RESERVED0 = 181,									\
	INPUT_ACTION_ORIGIN_XBOX_360_RESERVED1 = 182,									\
	INPUT_ACTION_ORIGIN_XBOX_360_RESERVED2 = 183,									\
    INPUT_ACTION_ORIGIN_XBOX_360_RESERVED3 = 184,									\
	INPUT_ACTION_ORIGIN_XBOX_360_RESERVED4 = 185,									\
	INPUT_ACTION_ORIGIN_XBOX_360_RESERVED5 = 186,									\
	INPUT_ACTION_ORIGIN_XBOX_360_RESERVED6 = 187,									\
	INPUT_ACTION_ORIGIN_XBOX_360_RESERVED7 = 188,									\
	INPUT_ACTION_ORIGIN_XBOX_360_RESERVED8 = 189,									\
	INPUT_ACTION_ORIGIN_XBOX_360_RESERVED9 = 190,									\
    INPUT_ACTION_ORIGIN_XBOX_360_RESERVED10 = 191,									\
	INPUT_ACTION_ORIGIN_SWITCH_A = 192,												\
	INPUT_ACTION_ORIGIN_SWITCH_B = 193,												\
	INPUT_ACTION_ORIGIN_SWITCH_X = 194,												\
	INPUT_ACTION_ORIGIN_SWITCH_Y = 195,												\
	INPUT_ACTION_ORIGIN_SWITCH_LEFT_BUMPER = 196,									\
	INPUT_ACTION_ORIGIN_SWITCH_RIGHT_BUMPER = 197,									\
	INPUT_ACTION_ORIGIN_SWITCH_PLUS = 198,											\
	INPUT_ACTION_ORIGIN_SWITCH_MINUS = 199,											\
																					\
    INPUT_ACTION_ORIGIN_SWITCH_CAPTURE = 200,										\
	INPUT_ACTION_ORIGIN_SWITCH_LEFT_TRIGGER_PULL = 201,								\
	INPUT_ACTION_ORIGIN_SWITCH_LEFT_TRIGGER_CLICK = 202,							\
	INPUT_ACTION_ORIGIN_SWITCH_RIGHT_TRIGGER_PULL = 203,							\
	INPUT_ACTION_ORIGIN_SWITCH_RIGHT_TRIGGER_CLICK = 204,							\
	INPUT_ACTION_ORIGIN_SWITCH_LEFT_STICK_MOVE = 205,								\
	INPUT_ACTION_ORIGIN_SWITCH_LEFT_STICK_CLICK = 206,								\
    INPUT_ACTION_ORIGIN_SWITCH_LEFT_STICK_DPAD_NORTH = 207,							\
	INPUT_ACTION_ORIGIN_SWITCH_LEFT_STICK_DPAD_SOUTH = 208,							\
	INPUT_ACTION_ORIGIN_SWITCH_LEFT_STICK_DPAD_WEST = 209,							\
	INPUT_ACTION_ORIGIN_SWITCH_LEFT_STICK_DPAD_EAST = 210,							\
	INPUT_ACTION_ORIGIN_SWITCH_RIGHT_STICK_MOVE = 211,								\
	INPUT_ACTION_ORIGIN_SWITCH_RIGHT_STICK_CLICK = 212,								\
    INPUT_ACTION_ORIGIN_SWITCH_RIGHT_STICK_DPAD_NORTH = 213,						\
	INPUT_ACTION_ORIGIN_SWITCH_RIGHT_STICK_DPAD_SOUTH = 214,						\
	INPUT_ACTION_ORIGIN_SWITCH_RIGHT_STICK_DPAD_WEST = 215,							\
	INPUT_ACTION_ORIGIN_SWITCH_RIGHT_STICK_DPAD_EAST = 216,							\
	INPUT_ACTION_ORIGIN_SWITCH_DPAD_NORTH = 217,									\
	INPUT_ACTION_ORIGIN_SWITCH_DPAD_SOUTH = 218,									\
	INPUT_ACTION_ORIGIN_SWITCH_DPAD_WEST = 219,										\
																					\
    INPUT_ACTION_ORIGIN_SWITCH_DPAD_EAST = 220,										\
	INPUT_ACTION_ORIGIN_SWITCH_PRO_GYRO_MOVE = 221,									\
	INPUT_ACTION_ORIGIN_SWITCH_PRO_GYRO_PITCH = 222,								\
	INPUT_ACTION_ORIGIN_SWITCH_PRO_GYRO_YAW = 223,									\
	INPUT_ACTION_ORIGIN_SWITCH_PRO_GYRO_ROLL = 224,									\
	INPUT_ACTION_ORIGIN_SWITCH_RESERVED0 = 225,										\
	INPUT_ACTION_ORIGIN_SWITCH_RESERVED1 = 226,										\
    INPUT_ACTION_ORIGIN_SWITCH_RESERVED2 = 227,										\
	INPUT_ACTION_ORIGIN_SWITCH_RESERVED3 = 228,										\
	INPUT_ACTION_ORIGIN_SWITCH_RESERVED4 = 229,										\
	INPUT_ACTION_ORIGIN_SWITCH_RESERVED5 = 230,										\
	INPUT_ACTION_ORIGIN_SWITCH_RESERVED6 = 231,										\
	INPUT_ACTION_ORIGIN_SWITCH_RESERVED7 = 232,										\
	INPUT_ACTION_ORIGIN_SWITCH_RESERVED8 = 233,										\
	INPUT_ACTION_ORIGIN_SWITCH_RESERVED9 = 234,										\
    INPUT_ACTION_ORIGIN_SWITCH_RESERVED10 = 235,									\
	INPUT_ACTION_ORIGIN_SWITCH_RIGHTGYRO_MOVE = 236,								\
	INPUT_ACTION_ORIGIN_SWITCH_RIGHTGYRO_PITCH = 237,								\
	INPUT_ACTION_ORIGIN_SWITCH_RIGHTGYRO_YAW = 238,									\
	INPUT_ACTION_ORIGIN_SWITCH_RIGHTGYRO_ROLL = 239,								\
	INPUT_ACTION_ORIGIN_SWITCH_LEFTGYRO_MOVE = 240,									\
	INPUT_ACTION_ORIGIN_SWITCH_LEFTGYRO_PITCH = 241,								\
    INPUT_ACTION_ORIGIN_SWITCH_LEFTGYRO_YAW = 242,									\
	INPUT_ACTION_ORIGIN_SWITCH_LEFTGYRO_ROLL = 243,									\
	INPUT_ACTION_ORIGIN_SWITCH_LEFTGRIP_LOWER = 244,								\
	INPUT_ACTION_ORIGIN_SWITCH_LEFTGRIP_UPPER = 245,								\
	INPUT_ACTION_ORIGIN_SWITCH_RIGHTGRIP_LOWER = 246,								\
	INPUT_ACTION_ORIGIN_SWITCH_RIGHTGRIP_UPPER = 247,								\
	INPUT_ACTION_ORIGIN_SWITCH_JOYCON_BUTTON_N = 248,								\
    INPUT_ACTION_ORIGIN_SWITCH_JOYCON_BUTTON_E = 249,								\
	INPUT_ACTION_ORIGIN_SWITCH_JOYCON_BUTTON_S = 250,								\
	INPUT_ACTION_ORIGIN_SWITCH_JOYCON_BUTTON_W = 251,								\
	INPUT_ACTION_ORIGIN_SWITCH_RESERVED15 = 252,									\
	INPUT_ACTION_ORIGIN_SWITCH_RESERVED16 = 253,									\
	INPUT_ACTION_ORIGIN_SWITCH_RESERVED17 = 254,									\
	INPUT_ACTION_ORIGIN_SWITCH_RESERVED18 = 255,									\
    INPUT_ACTION_ORIGIN_SWITCH_RESERVED19 = 256,									\
	INPUT_ACTION_ORIGIN_SWITCH_RESERVED20 = 257,									\
	INPUT_ACTION_ORIGIN_PS5_X = 258,												\
	INPUT_ACTION_ORIGIN_PS5_CIRCLE = 259,											\
	INPUT_ACTION_ORIGIN_PS5_TRIANGLE = 260,											\
	INPUT_ACTION_ORIGIN_PS5_SQUARE = 261,											\
	INPUT_ACTION_ORIGIN_PS5_LEFTBUMPER = 262,										\
	INPUT_ACTION_ORIGIN_PS5_RIGHTBUMPER = 263,										\
	INPUT_ACTION_ORIGIN_PS5_OPTION = 264,											\
																					\
    INPUT_ACTION_ORIGIN_PS5_CREATE = 265,											\
	INPUT_ACTION_ORIGIN_PS5_MUTE = 266,												\
	INPUT_ACTION_ORIGIN_PS5_LEFTPAD_TOUCH = 267,									\
	INPUT_ACTION_ORIGIN_PS5_LEFTPAD_SWIPE = 268,									\
	INPUT_ACTION_ORIGIN_PS5_LEFTPAD_CLICK = 269,									\
	INPUT_ACTION_ORIGIN_PS5_LEFTPAD_DPADNORTH = 270,								\
	INPUT_ACTION_ORIGIN_PS5_LEFTPAD_DPADSOUTH = 271,								\
	INPUT_ACTION_ORIGIN_PS5_LEFTPAD_DPADWEST = 272,									\
																					\
    INPUT_ACTION_ORIGIN_PS5_LEFTPAD_DPADEAST = 273,									\
	INPUT_ACTION_ORIGIN_PS5_RIGHTPAD_TOUCH = 274,									\
	INPUT_ACTION_ORIGIN_PS5_RIGHTPAD_SWIPE = 275,									\
	INPUT_ACTION_ORIGIN_PS5_RIGHTPAD_CLICK = 276,									\
	INPUT_ACTION_ORIGIN_PS5_RIGHTPAD_DPADNORTH = 277,								\
	INPUT_ACTION_ORIGIN_PS5_RIGHTPAD_DPADSOUTH = 278,								\
	INPUT_ACTION_ORIGIN_PS5_RIGHTPAD_DPADWEST = 279,								\
    INPUT_ACTION_ORIGIN_PS5_RIGHTPAD_DPADEAST = 280,								\
	INPUT_ACTION_ORIGIN_PS5_CENTERPAD_TOUCH = 281,									\
	INPUT_ACTION_ORIGIN_PS5_CENTERPAD_SWIPE = 282,									\
	INPUT_ACTION_ORIGIN_PS5_CENTERPAD_CLICK = 283,									\
	INPUT_ACTION_ORIGIN_PS5_CENTERPAD_DPADNORTH = 284,								\
	INPUT_ACTION_ORIGIN_PS5_CENTERPAD_DPADSOUTH = 285,								\
	INPUT_ACTION_ORIGIN_PS5_CENTERPAD_DPADWEST = 286,								\
    INPUT_ACTION_ORIGIN_PS5_CENTERPAD_DPADEAST = 287,								\
	INPUT_ACTION_ORIGIN_PS5_LEFTTRIGGER_PULL = 288,									\
	INPUT_ACTION_ORIGIN_PS5_LEFTTRIGGER_CLICK = 289,								\
	INPUT_ACTION_ORIGIN_PS5_RIGHTTRIGGER_PULL = 290,								\
	INPUT_ACTION_ORIGIN_PS5_RIGHTTRIGGER_CLICK = 291,								\
	INPUT_ACTION_ORIGIN_PS5_LEFTSTICK_MOVE = 292,									\
	INPUT_ACTION_ORIGIN_PS5_LEFTSTICK_CLICK = 293,									\
    INPUT_ACTION_ORIGIN_PS5_LEFTSTICK_DPADNORTH = 294,								\
	INPUT_ACTION_ORIGIN_PS5_LEFTSTICK_DPADSOUTH = 295,								\
	INPUT_ACTION_ORIGIN_PS5_LEFTSTICK_DPADWEST = 296,								\
	INPUT_ACTION_ORIGIN_PS5_LEFTSTICK_DPADEAST = 297,								\
	INPUT_ACTION_ORIGIN_PS5_RIGHTSTICK_MOVE = 298,									\
	INPUT_ACTION_ORIGIN_PS5_RIGHTSTICK_CLICK = 299,									\
	INPUT_ACTION_ORIGIN_PS5_RIGHTSTICK_DPADNORTH = 300,								\
    INPUT_ACTION_ORIGIN_PS5_RIGHTSTICK_DPADSOUTH = 301,								\
	INPUT_ACTION_ORIGIN_PS5_RIGHTSTICK_DPADWEST = 302,								\
	INPUT_ACTION_ORIGIN_PS5_RIGHTSTICK_DPADEAST = 303,								\
	INPUT_ACTION_ORIGIN_PS5_DPAD_NORTH = 304,										\
	INPUT_ACTION_ORIGIN_PS5_DPAD_SOUTH = 305,										\
	INPUT_ACTION_ORIGIN_PS5_DPAD_WEST = 306,										\
	INPUT_ACTION_ORIGIN_PS5_DPAD_EAST = 307,										\
	INPUT_ACTION_ORIGIN_PS5_GYRO_MOVE = 308,										\
    INPUT_ACTION_ORIGIN_PS5_GYRO_PITCH = 309,										\
	INPUT_ACTION_ORIGIN_PS5_GYRO_YAW = 310,											\
	INPUT_ACTION_ORIGIN_PS5_GYRO_ROLL = 311,										\
	INPUT_ACTION_ORIGIN_PS5_DPAD_MOVE = 312,										\
	INPUT_ACTION_ORIGIN_PS5_LEFTGRIP = 313,											\
	INPUT_ACTION_ORIGIN_PS5_RIGHTGRIP = 314,										\
	INPUT_ACTION_ORIGIN_PS5_LEFTFN = 315,											\
	INPUT_ACTION_ORIGIN_PS5_RIGHTFN = 316,											\
	INPUT_ACTION_ORIGIN_PS5_RESERVED5 = 317,										\
    INPUT_ACTION_ORIGIN_PS5_RESERVED6 = 318,										\
	INPUT_ACTION_ORIGIN_PS5_RESERVED7 = 319,										\
	INPUT_ACTION_ORIGIN_PS5_RESERVED8 = 320,										\
	INPUT_ACTION_ORIGIN_PS5_RESERVED9 = 321,										\
	INPUT_ACTION_ORIGIN_PS5_RESERVED10 = 322,										\
	INPUT_ACTION_ORIGIN_PS5_RESERVED11 = 323,										\
	INPUT_ACTION_ORIGIN_PS5_RESERVED12 = 324,										\
	INPUT_ACTION_ORIGIN_PS5_RESERVED13 = 325,										\
    INPUT_ACTION_ORIGIN_PS5_RESERVED14 = 326,										\
	INPUT_ACTION_ORIGIN_PS5_RESERVED15 = 327,										\
	INPUT_ACTION_ORIGIN_PS5_RESERVED16 = 328,										\
	INPUT_ACTION_ORIGIN_PS5_RESERVED17 = 329,										\
	INPUT_ACTION_ORIGIN_PS5_RESERVED18 = 330,										\
	INPUT_ACTION_ORIGIN_PS5_RESERVED19 = 331,										\
	INPUT_ACTION_ORIGIN_PS5_RESERVED20 = 332,										\
	INPUT_ACTION_ORIGIN_STEAMDECK_A = 333,											\
    INPUT_ACTION_ORIGIN_STEAMDECK_B = 334,											\
	INPUT_ACTION_ORIGIN_STEAMDECK_X = 335,											\
	INPUT_ACTION_ORIGIN_STEAMDECK_Y = 336,											\
	INPUT_ACTION_ORIGIN_STEAMDECK_L1 = 337,											\
	INPUT_ACTION_ORIGIN_STEAMDECK_R1 = 338,											\
	INPUT_ACTION_ORIGIN_STEAMDECK_MENU = 339,										\
	INPUT_ACTION_ORIGIN_STEAMDECK_VIEW = 340,										\
	INPUT_ACTION_ORIGIN_STEAMDECK_LEFTPAD_TOUCH = 341,								\
    INPUT_ACTION_ORIGIN_STEAMDECK_LEFTPAD_SWIPE = 342,								\
	INPUT_ACTION_ORIGIN_STEAMDECK_LEFTPAD_CLICK = 343,								\
	INPUT_ACTION_ORIGIN_STEAMDECK_LEFTPAD_DPADNORTH = 344,							\
	INPUT_ACTION_ORIGIN_STEAMDECK_LEFTPAD_DPADSOUTH = 345,							\
	INPUT_ACTION_ORIGIN_STEAMDECK_LEFTPAD_DPADWEST = 346,							\
	INPUT_ACTION_ORIGIN_STEAMDECK_LEFTPAD_DPADEAST = 347,							\
    INPUT_ACTION_ORIGIN_STEAMDECK_RIGHTPAD_TOUCH = 348,								\
	INPUT_ACTION_ORIGIN_STEAMDECK_RIGHTPAD_SWIPE = 349,								\
	INPUT_ACTION_ORIGIN_STEAMDECK_RIGHTPAD_CLICK = 350,								\
	INPUT_ACTION_ORIGIN_STEAMDECK_RIGHTPAD_DPADNORTH = 351,							\
	INPUT_ACTION_ORIGIN_STEAMDECK_RIGHTPAD_DPADSOUTH = 352,							\
	INPUT_ACTION_ORIGIN_STEAMDECK_RIGHTPAD_DPADWEST = 353,							\
    INPUT_ACTION_ORIGIN_STEAMDECK_RIGHTPAD_DPADEAST = 354,							\
	INPUT_ACTION_ORIGIN_STEAMDECK_L2_SOFTPULL = 355,								\
	INPUT_ACTION_ORIGIN_STEAMDECK_L2 = 356,											\
	INPUT_ACTION_ORIGIN_STEAMDECK_R2_SOFTPULL = 357,								\
	INPUT_ACTION_ORIGIN_STEAMDECK_R2 = 358,											\
	INPUT_ACTION_ORIGIN_STEAMDECK_LEFTSTICK_MOVE = 359,								\
	INPUT_ACTION_ORIGIN_STEAMDECK_L3 = 360,											\
    INPUT_ACTION_ORIGIN_STEAMDECK_LEFTSTICK_DPADNORTH = 361,						\
	INPUT_ACTION_ORIGIN_STEAMDECK_LEFTSTICK_DPADSOUTH = 362,						\
	INPUT_ACTION_ORIGIN_STEAMDECK_LEFTSTICK_DPADWEST = 363,							\
	INPUT_ACTION_ORIGIN_STEAMDECK_LEFTSTICK_DPADEAST = 364,							\
	INPUT_ACTION_ORIGIN_STEAMDECK_LEFTSTICK_TOUCH = 365,							\
	INPUT_ACTION_ORIGIN_STEAMDECK_RIGHTSTICK_MOVE = 366,							\
    INPUT_ACTION_ORIGIN_STEAMDECK_R3 = 367,											\
	INPUT_ACTION_ORIGIN_STEAMDECK_RIGHTSTICK_DPADNORTH = 368,						\
	INPUT_ACTION_ORIGIN_STEAMDECK_RIGHTSTICK_DPADSOUTH = 369,						\
	INPUT_ACTION_ORIGIN_STEAMDECK_RIGHTSTICK_DPADWEST = 370,						\
	INPUT_ACTION_ORIGIN_STEAMDECK_RIGHTSTICK_DPADEAST = 371,						\
	INPUT_ACTION_ORIGIN_STEAMDECK_RIGHTSTICK_TOUCH = 372,							\
	INPUT_ACTION_ORIGIN_STEAMDECK_L4 = 373,											\
    INPUT_ACTION_ORIGIN_STEAMDECK_R4 = 374,											\
	INPUT_ACTION_ORIGIN_STEAMDECK_L5 = 375,											\
	INPUT_ACTION_ORIGIN_STEAMDECK_R5 = 376,											\
	INPUT_ACTION_ORIGIN_STEAMDECK_DPAD_MOVE = 377,									\
	INPUT_ACTION_ORIGIN_STEAMDECK_DPAD_NORTH = 378,									\
	INPUT_ACTION_ORIGIN_STEAMDECK_DPAD_SOUTH = 379,									\
	INPUT_ACTION_ORIGIN_STEAMDECK_DPAD_WEST = 380,									\
	INPUT_ACTION_ORIGIN_STEAMDECK_DPAD_EAST = 381,									\
    INPUT_ACTION_ORIGIN_STEAMDECK_GYRO_MOVE = 382,									\
	INPUT_ACTION_ORIGIN_STEAMDECK_GYRO_PITCH = 383,									\
	INPUT_ACTION_ORIGIN_STEAMDECK_GYRO_YAW = 384,									\
	INPUT_ACTION_ORIGIN_STEAMDECK_GYRO_ROLL = 385,									\
	INPUT_ACTION_ORIGIN_STEAMDECK_RESERVED1 = 386,									\
	INPUT_ACTION_ORIGIN_STEAMDECK_RESERVED2 = 387,									\
	INPUT_ACTION_ORIGIN_STEAMDECK_RESERVED3 = 388,									\
    INPUT_ACTION_ORIGIN_STEAMDECK_RESERVED4 = 389,									\
	INPUT_ACTION_ORIGIN_STEAMDECK_RESERVED5 = 390,									\
	INPUT_ACTION_ORIGIN_STEAMDECK_RESERVED6 = 391,									\
	INPUT_ACTION_ORIGIN_STEAMDECK_RESERVED7 = 392,									\
	INPUT_ACTION_ORIGIN_STEAMDECK_RESERVED8 = 393,									\
	INPUT_ACTION_ORIGIN_STEAMDECK_RESERVED9 = 394,									\
	INPUT_ACTION_ORIGIN_STEAMDECK_RESERVED10 = 395,									\
    INPUT_ACTION_ORIGIN_STEAMDECK_RESERVED11 = 396,									\
	INPUT_ACTION_ORIGIN_STEAMDECK_RESERVED12 = 397,									\
	INPUT_ACTION_ORIGIN_STEAMDECK_RESERVED13 = 398,									\
	INPUT_ACTION_ORIGIN_STEAMDECK_RESERVED14 = 399,									\
	INPUT_ACTION_ORIGIN_STEAMDECK_RESERVED15 = 400,									\
	INPUT_ACTION_ORIGIN_STEAMDECK_RESERVED16 = 401,									\
	INPUT_ACTION_ORIGIN_STEAMDECK_RESERVED17 = 402,									\
    INPUT_ACTION_ORIGIN_STEAMDECK_RESERVED18 = 403,									\
	INPUT_ACTION_ORIGIN_STEAMDECK_RESERVED19 = 404,									\
	INPUT_ACTION_ORIGIN_STEAMDECK_RESERVED20 = 405,									\
	INPUT_ACTION_ORIGIN_COUNT = 406,												\
	INPUT_ACTION_ORIGIN_MAXIMUMPOSSIBLEVALUE = 32767								\
};																					\
enum SteamInputType {																\
    INPUT_TYPE_UNKNOWN = 0,															\
	INPUT_TYPE_STEAM_CONTROLLER = 1,												\
	INPUT_TYPE_XBOX360_CONTROLLER = 2,												\
	INPUT_TYPE_XBOXONE_CONTROLLER = 3,												\
	INPUT_TYPE_GENERIC_XINPUT = 4,													\
	INPUT_TYPE_PS4_CONTROLLER = 5,													\
	INPUT_TYPE_APPLE_MFI_CONTROLLER = 6,											\
	INPUT_TYPE_ANDROID_CONTROLLER = 7,												\
	INPUT_TYPE_SWITCH_JOYCON_PAIR = 8,												\
	INPUT_TYPE_SWITCH_JOYCON_SINGLE = 9,											\
    INPUT_TYPE_SWITCH_PRO_CONTROLLER = 10,											\
	INPUT_TYPE_MOBILE_TOUCH = 11,													\
	INPUT_TYPE_PS3_CONTROLLER = 12,													\
	INPUT_TYPE_PS5_CONTROLLER = 13,													\
	INPUT_TYPE_STEAM_DECK_CONTROLLER = 14,											\
	INPUT_TYPE_COUNT = 15,															\
	INPUT_TYPE_MAXIMUM_POSSIBLE_VALUE = 255											\
};																					\
enum ConfigurationEnableType {														\
    INPUT_CONFIGURATION_ENABLE_TYPE_NONE = 0x0000,									\
	INPUT_CONFIGURATION_ENABLE_TYPE_PLAYSTATION = 0x0001,							\
	INPUT_CONFIGURATION_ENABLE_TYPE_XBOX = 0x0002,									\
	INPUT_CONFIGURATION_ENABLE_TYPE_GENERIC = 0x0004,								\
	INPUT_CONFIGURATION_ENABLE_TYPE_SWITCH = 0x0008									\
};																					\
enum GlyphSize {																	\
    INPUT_GLYPH_SIZE_SMALL = 0,														\
	INPUT_GLYPH_SIZE_MEDIUM = 1,													\
	INPUT_GLYPH_SIZE_LARGE = 2,														\
	INPUT_GLYPH_SIZE_COUNT = 3														\
};																					\
enum GlyphStyle {																	\
    INPUT_GLYPH_STYLE_KNOCKOUT = 0x0,												\
	INPUT_GLYPH_STYLE_LIGHT = 0x1,													\
	INPUT_GLYPH_STYLE_DARK = 0x2,													\
	INPUT_GLYPH_STYLE_NEUTRAL_COLOR_ABXY = 0x10,									\
	INPUT_GLYPH_STYLE_SOLID_ABXY = 0x20												\
};																					\
enum SCEPadTriggerEffectMode {														\
    PAD_TRIGGER_EFFECT_MODE_OFF = 0,												\
	PAD_TRIGGER_EFFECT_MODE_FEEDBACK = 1,											\
	PAD_TRIGGER_EFFECT_MODE_WEAPON = 2,												\
	PAD_TRIGGER_EFFECT_MODE_VIBRATION = 3,											\
	PAD_TRIGGER_EFFECT_MODE_MULTIPLE_POSITION_FEEDBACK = 4,							\
	PAD_TRIGGER_EFFECT_MODE_SLOPE_FEEDBACK = 5,										\
	PAD_TRIGGER_EFFECT_MODE_MULTIPLE_POSITION_VIBRATION = 6							\
};																					\
																					\
/* Inventory enums*/																\
enum SteamItemFlags {																\
    STEAM_ITEM_NO_TRADE = (1<<0),													\
	STEAM_ITEM_REMOVED = (1<<8),													\
	STEAM_ITEM_CONSUMED = (1<<9)													\
};																					\
																					\
/* Matchmaking enums*/																\
enum ChatMemberStateChange {														\
    CHAT_MEMBER_STATE_CHANGE_ENTERED = 0x0001,										\
	CHAT_MEMBER_STATE_CHANGE_LEFT = 0x0002,											\
	CHAT_MEMBER_STATE_CHANGE_DISCONNECTED = 0x0004,									\
	CHAT_MEMBER_STATE_CHANGE_KICKED = 0x0008,										\
	CHAT_MEMBER_STATE_CHANGE_BANNED = 0x0010										\
};																					\
enum LobbyComparison {																\
    LOBBY_COMPARISON_EQUAL_TO_OR_LESS_THAN = -2,									\
	LOBBY_COMPARISON_LESS_THAN = -1,												\
	LOBBY_COMPARISON_EQUAL = 0,														\
	LOBBY_COMPARISON_GREATER_THAN = 1,												\
	LOBBY_COMPARISON_EQUAL_TO_GREATER_THAN = 2,										\
	LOBBY_COMPARISON_NOT_EQUAL = 3													\
};																					\
enum LobbyDistanceFilter {															\
    LOBBY_DISTANCE_FILTER_CLOSE = 0,												\
	LOBBY_DISTANCE_FILTER_DEFAULT = 1,												\
	LOBBY_DISTANCE_FILTER_FAR = 2,													\
	LOBBY_DISTANCE_FILTER_WORLDWIDE = 3												\
};																					\
enum LobbyType {																	\
    LOBBY_TYPE_PRIVATE = 0,															\
	LOBBY_TYPE_FRIENDS_ONLY = 1,													\
	LOBBY_TYPE_PUBLIC = 2,															\
	LOBBY_TYPE_INVISIBLE = 3														\
};																					\
																					\
/* Matchmaking Servers enums */														\
enum MatchMakingServerResponse {													\
    SERVER_RESPONDED = 0,															\
	SERVER_FAILED_TO_RESPOND = 1,													\
	NO_SERVERS_LISTED_ON_MASTER_SERVER = 2											\
};																					\
																					\
/* Music enums*/																	\
enum AudioPlaybackStatus {															\
    AUDIO_PLAYBACK_UNDEFINED = 0,													\
	AUDIO_PLAYBACK_PLAYING = 1,														\
	AUDIO_PLAYBACK_PAUSED = 2,														\
	AUDIO_PLAYBACK_IDLE = 3															\
};																					\
																					\
/* Networking enums*/																\
enum P2PSend {																		\
    P2P_SEND_UNRELIABLE = 0,														\
	P2P_SEND_UNRELIABLE_NO_DELAY = 1,												\
	P2P_SEND_RELIABLE = 2,															\
	P2P_SEND_RELIABLE_WITH_BUFFERING = 3											\
};																					\
enum P2PSessionError {																\
    P2P_SESSION_ERROR_NONE = 0,														\
	P2P_SESSION_ERROR_NOT_RUNNING_APP = 1,											\
	P2P_SESSION_ERROR_NO_RIGHTS_TO_APP = 2,											\
	P2P_SESSION_ERROR_DESTINATION_NOT_LOGGED_ON = 3,								\
	P2P_SESSION_ERROR_TIMEOUT = 4,													\
	P2P_SESSION_ERROR_MAX = 5														\
};																					\
enum SNetSocketConnectionType {														\
    NET_SOCKET_CONNECTION_TYPE_NOT_CONNECTED = 0,									\
	NET_SOCKET_CONNECTION_TYPE_UDP = 1,												\
	NET_SOCKET_CONNECTION_TYPE_UDP_RELAY = 2										\
};																					\
enum SNetSocketState {																\
    NET_SOCKET_STATE_INVALID = 0,													\
	NET_SOCKET_STATE_CONNECTED = 1,													\
	NET_SOCKET_STATE_INITIATED = 10,												\
	NET_SOCKET_STATE_LOCAL_CANDIDATE_FOUND = 11,									\
	NET_SOCKET_STATE_RECEIVED_REMOTE_CANDIDATES = 12,								\
	NET_SOCKET_STATE_CHALLENGE_HANDSHAKE = 15,										\
	NET_SOCKET_STATE_DISCONNECTING = 21,											\
	NET_SOCKET_STATE_LOCAL_DISCONNECT = 22,											\
	NET_SOCKET_STATE_TIMEOUT_DURING_CONNECT = 23,									\
    NET_SOCKET_STATE_REMOTE_END_DISCONNECTED = 24,									\
	NET_SOCKET_STATE_BROKEN = 25													\
};																					\
																					\
/* Networking Sockets enums*/														\
enum NetworkingConfigValue {														\
    NETWORKING_CONFIG_INVALID = 0,													\
	NETWORKING_CONFIG_FAKE_PACKET_LOSS_SEND = 2,									\
	NETWORKING_CONFIG_FAKE_PACKET_LOSS_RECV = 3,									\
	NETWORKING_CONFIG_FAKE_PACKET_LAG_SEND = 4,										\
	NETWORKING_CONFIG_FAKE_PACKET_LAG_RECV = 5,										\
	NETWORKING_CONFIG_FAKE_PACKET_REORDER_SEND = 6,									\
	NETWORKING_CONFIG_FAKE_PACKET_REORDER_RECV = 7,									\
	NETWORKING_CONFIG_FAKE_PACKET_REORDER_TIME = 8,									\
    NETWORKING_CONFIG_FAKE_PACKET_DUP_SEND = 26,									\
	NETWORKING_CONFIG_FAKE_PACKET_DUP_REVC = 27,									\
	NETWORKING_CONFIG_FAKE_PACKET_DUP_TIME_MAX = 28,								\
	NETWORKING_CONFIG_TIMEOUT_INITIAL = 24,											\
	NETWORKING_CONFIG_TIMEOUT_CONNECTED = 25,										\
	NETWORKING_CONFIG_SEND_BUFFER_SIZE = 9,											\
	NETWORKING_CONFIG_SEND_RATE_MIN = 10,											\
	NETWORKING_CONFIG_SEND_RATE_MAX = 11,											\
    NETWORKING_CONFIG_NAGLE_TIME = 12,												\
	NETWORKING_CONFIG_IP_ALLOW_WITHOUT_AUTH = 23,									\
	NETWORKING_CONFIG_SDR_CLIENT_CONSEC_PING_TIMEOUT_FAIL_INITIAL = 19,				\
	NETWORKING_CONFIG_SDR_CLIENT_CONSEC_PING_TIMEOUT_FAIL = 20,						\
	NETWORKING_CONFIG_SDR_CLIENT_MIN_PINGS_BEFORE_PING_ACCURATE = 21,				\
	NETWORKING_CONFIG_SDR_CLIENT_SINGLE_SOCKET = 22,								\
    NETWORKING_CONFIG_SDR_CLIENT_FORCE_RELAY_CLUSTER = 29,							\
	NETWORKING_CONFIG_SDR_CLIENT_DEBUG_TICKET_ADDRESS = 30,							\
	NETWORKING_CONFIG_SDR_CLIENT_FORCE_PROXY_ADDR = 31,								\
	NETWORKING_CONFIG_LOG_LEVEL_ACK_RTT = 13,										\
	NETWORKING_CONFIG_LOG_LEVEL_PACKET_DECODE = 14,									\
	NETWORKING_CONFIG_LOG_LEVEL_MESSAGE = 15,										\
	NETWORKING_CONFIG_LOG_LEVEL_PACKET_GAPS = 16,									\
    NETWORKING_CONFIG_LOG_LEVEL_P2P_RENDEZVOUS = 17,								\
	NETWORKING_CONFIG_LOG_LEVEL_SRD_RELAY_PINGS = 18								\
};																					\
enum NetworkingGetConfigValueResult {												\
    NETWORKING_GET_CONFIG_VALUE_BAD_VALUE = -1,										\
	NETWORKING_GET_CONFIG_VALUE_BAD_SCOPE_OBJ = -2,									\
	NETWORKING_GET_CONFIG_VALUE_BUFFER_TOO_SMALL = -3,								\
	NETWORKING_GET_CONFIG_VALUE_OK = 1,												\
	NETWORKING_GET_CONFIG_VALUE_OK_INHERITED = 2,									\
	NETWORKING_GET_CONFIG_VALUE_FORCE_32BIT = 0x7fffffff							\
};																					\
enum NetworkingConnectionState {													\
    CONNECTION_STATE_NONE = 0,														\
	CONNECTION_STATE_CONNECTING = 1,												\
	CONNECTION_STATE_FINDING_ROUTE = 2,												\
	CONNECTION_STATE_CONNECTED = 3,													\
	CONNECTION_STATE_CLOSED_BY_PEER = 4,											\
	CONNECTION_STATE_PROBLEM_DETECTED_LOCALLY = 5,									\
	CONNECTION_STATE_FIN_WAIT = -1,													\
	CONNECTION_STATE_LINGER = -2,													\
	CONNECTION_STATE_DEAD = -3,														\
	CONNECTION_STATE_FORCE32BIT = 0x7fffffff										\
};																					\
enum NetworkingConnectionEnd {														\
    CONNECTION_END_INVALID = 0,														\
	CONNECTION_END_APP_MIN = 1000,													\
	CONNECTION_END_MAX = 1999,														\
	CONNECTION_END_APP_EXCEPTION_MIN = 2000,										\
	CONNECTION_END_APP_EXCEPTION_MAX = 2999,										\
	CONNECTION_END_LOCAL_MIN = 3000,												\
	CONNECTION_END_LOCAL_OFFLINE_MODE = 3001,										\
	CONNECTION_END_LOCAL_MANY_RELAY_CONNECTIVITY = 3002,							\
	CONNECTION_END_LOCAL_HOSTED_sERVER_PRIMARY_RELAY = 3003,						\
    CONNECTION_END_LOCAL_NETWORK_CONFIG = 3004,										\
	CONNECTION_END_LOCAL_RIGHTS = 3005,												\
	CONNECTION_END_LOCAL_MAX = 3999,												\
	CONNECTION_END_REMOVE_MIN = 4000,												\
	CONNECTION_END_REMOTE_TIMEOUT = 4001,											\
	CONNECTION_END_REMOTE_BAD_CRYPT = 4002,											\
	CONNECTION_END_REMOTE_BAD_CERT = 4003,											\
	CONNECTION_END_REMOTE_NOT_LOGGED_IN = 4004,										\
	CONNECTION_END_REMOTE_NOT_RUNNING_APP = 4005,									\
    CONNECTION_END_BAD_PROTOCOL_VERSION = 4006,										\
	CONNECTION_END_REMOTE_MAX = 4999,												\
	CONNECTION_END_MISC_MIN = 5000,													\
	CONNECTION_END_MISC_GENERIC = 5001,												\
	CONNECTION_END_MISC_INTERNAL_ERROR = 5002,										\
	CONNECTION_END_MISC_TIMEOUT = 5003,												\
	CONNECTION_END_MISC_RELAY_CONNECTIVITY = 5004,									\
	CONNECTION_END_MISC_STEAM_CONNECTIVITY = 5005,									\
    CONNECTION_END_MISC_NO_RELAY_SESSIONS_TO_CLIENT = 5006,							\
	CONNECTION_END_MISC_MAX = 5999													\
};																					\
enum NetworkingIdentityType {														\
    IDENTITY_TYPE_INVALID = 0,														\
	IDENTITY_TYPE_STEAMID = 16,														\
	IDENTITY_TYPE_IP_ADDRESS = 1,													\
	IDENTITY_TYPE_GENERIC_STRING = 2,												\
	IDENTITY_TYPE_GENERIC_BYTES = 3,												\
	IDENTITY_TYPE_UNKNOWN_TYPE = 4,													\
	IDENTITY_TYPE_XBOX_PAIRWISE = 17,												\
	IDENTITY_TYPE_SONY_PSN = 18,													\
	IDENTITY_TYPE_GOOGLE_STADIA = 19,												\
	IDENTITY_TYPE_FORCE_32BIT = 0x7fffffff											\
};																					\
enum NetworkingSocketsDebugOutputType {												\
    NETWORKING_SOCKET_DEBUG_OUTPUT_TYPE_NONE = 0,									\
	NETWORKING_SOCKET_DEBUG_OUTPUT_TYPE_BUG = 1,									\
	NETWORKING_SOCKET_DEBUG_OUTPUT_TYPE_ERROR = 2,									\
	NETWORKING_SOCKET_DEBUG_OUTPUT_TYPE_IMPORTANT = 3,								\
	NETWORKING_SOCKET_DEBUG_OUTPUT_TYPE_WARNING = 4,								\
	NETWORKING_SOCKET_DEBUG_OUTPUT_TYPE_MSG = 5,									\
	NETWORKING_SOCKET_DEBUG_OUTPUT_TYPE_VERBOSE = 6,								\
    NETWORKING_SOCKET_DEBUG_OUTPUT_TYPE_DEBUG = 7,									\
	NETWORKING_SOCKET_DEBUG_OUTPUT_TYPE_EVERYTHING = 8,								\
	NETWORKING_SOCKET_DEBUG_OUTPUT_TYPE_FORCE_32BIT = 0x7fffffff					\
};																					\
																					\
/* Networking Utils enums*/ 														\
enum NetworkingAvailability {														\
    NETWORKING_AVAILABILITY_CANNOT_TRY = -102,										\
	NETWORKING_AVAILABILITY_FAILED = -101,											\
	NETWORKING_AVAILABILITY_PREVIOUSLY = -100,										\
	NETWORKING_AVAILABILITY_NEVER_TRIED = 1,										\
	NETWORKING_AVAILABILITY_WAITING = 2,											\
	NETWORKING_AVAILABILITY_ATTEMPTING = 3,											\
	NETWORKING_AVAILABILITY_CURRENT = 100,											\
	NETWORKING_AVAILABILITY_UNKNOWN = 0,											\
    NETWORKING_AVAILABILITY_FORCE_32BIT = 0x7fffffff								\
};																					\
enum NetworkingConfigScope {														\
    NETWORKING_CONFIG_SCOPE_GLOBAL = 1,												\
	NETWORKING_CONFIG_SCOPE_SOCKETS_INTERFACE = 2,									\
	NETWORKING_CONFIG_SCOPE_LISTEN_SOCKET = 3,										\
	NETWORKING_CONFIG_SCOPE_CONNECTION = 4,											\
	NETWORKING_CONFIG_SCOPE_FORCE_32BIT = 0x7fffffff								\
};																					\
enum NetworkingConfigDataType {														\
    NETWORKING_CONFIG_TYPE_INT32 = 1,												\
	NETWORKING_CONFIG_TYPE_INT64 = 2,												\
	NETWORKING_CONFIG_TYPE_FLOAT = 3,												\
	NETWORKING_CONFIG_TYPE_STRING = 4,												\
	NETWORKING_CONFIG_TYPE_FUNCTION_PTR = 5,										\
	NETWORKING_CONFIG_TYPE_FORCE_32BIT = 0x7fffffff									\
};																					\
																					\
/* Parental Settings enums*/														\
enum ParentalFeature {																\
    FEATURE_INVALID = 0,															\
	FEATURE_STORE = 1,																\
	FEATURE_COMMUNITY = 2,															\
	FEATURE_PROFILE = 3,															\
	FEATURE_FRIENDS = 4,															\
	FEATURE_NEWS = 5,																\
	FEATURE_TRADING = 6,															\
	FEATURE_SETTINGS = 7,															\
	FEATURE_CONSOLE = 8,															\
	FEATURE_BROWSER = 9,															\
	FEATURE_PARENTAL_SETUP = 10,													\
	FEATURE_LIBRARY = 11,															\
	FEATURE_TEST = 12,																\
	FEATURE_SITE_LICENSE = 13,														\
	FEATURE_KIOSK_MODE = 14,														\
	FEATURE_MAX																		\
};																					\
																					\
/* Steam Parties enums*/															\
enum SteamPartyBeaconLocationType {													\
    STEAM_PARTY_BEACON_LOCATIONTYPE_INVALID = 0,									\
	STEAM_PARTY_BEACON_LOCATIONTYPE_CHAT_GROUP = 1,									\
	STEAM_PARTY_BEACON_LOCATION_TYPE_MAX											\
};																					\
enum SteamPartyBeaconLocationData {													\
    STEAM_PARTY_BEACON_LOCATION_DATA = 0,											\
	STEAM_PARTY_BEACON_LOCATION_DATA_NAME = 1,										\
	STEAM_PARTY_BEACON_LOCATION_DATA_URL_SMALL,										\
	STEAM_PARTY_BEACON_LOCATION_DATA_URL_MEDIUM,									\
	STEAM_PARTY_BEACON_LOCATION_DATA_URL_LARGE										\
};																					\
																					\
/* Remote Storage enums*/															\
enum RemoteStoragePlatform {														\
    REMOTE_STORAGE_PLATFORM_NONE = 0,												\
	REMOTE_STORAGE_PLATFORM_WINDOWS = (1<<0),										\
	REMOTE_STORAGE_PLATFORM_OSX = (1<<1),											\
	REMOTE_STORAGE_PLATFORM_PS3 = (1<<2),											\
	REMOTE_STORAGE_PLATFORM_LINUX = (1<<3),											\
	REMOTE_STORAGE_PLATFORM_RESERVED2 = (1<<4),										\
	REMOTE_STORAGE_PLATFORM_ALL = 0XFFFFFFFF										\
};																					\
enum RemoteStoragePublishedFileVisibility {											\
    REMOTE_STORAGE_PUBLISHED_VISIBLITY_PUBLIC = 0,									\
	REMOTE_STORAGE_PUBLISHED_VISIBLITY_FRIENDS_ONLY = 1,							\
	REMOTE_STORAGE_PUBLISHED_VISIBLITY_PRIVATE = 2									\
};																					\
enum UGCReadAction {																\
    UGC_READ_CONTINUE_READING_UNTIL_FINISHED = 0,									\
	UGC_READ_CONTINUE_READING = 1,													\
	UGC_READ_CLOSE = 2																\
};																					\
enum WorkshopEnumerationType {														\
    WORKSHOP_ENUMERATION_TYPE_RANKED_BY_VOTE = 0,									\
	WORKSHOP_ENUMERATION_TYPE_RECENT = 1,											\
	WORKSHOP_ENUMERATION_TYPE_TRENDING = 2,											\
	WORKSHOP_ENUMERATION_TYPE_FAVORITES_OF_FRIENDS = 3,								\
	WORKSHOP_ENUMERATION_TYPE_VOTED_BY_FRIENDS = 4,									\
	WORKSHOP_ENUMERATION_TYPE_CONTENT_BY_FRIENDS = 5,								\
	WORKSHOP_ENUMERATION_TYPE_RECENT_FROM_FOLLOWED_USERS = 6						\
};																					\
enum WorkshopFileAction {															\
    WORKSHOP_FILE_ACTION_PLAYED = 0,												\
	WORKSHOP_FILE_ACTION_COMPLETED = 1												\
};																					\
enum WorkshopFileType {																\
    WORKSHOP_FILE_TYPE_FIRST = 0,													\
	WORKSHOP_FILE_TYPE_COMMUNITY = 0,												\
	WORKSHOP_FILE_TYPE_MICROTRANSACTION = 1,										\
	WORKSHOP_FILE_TYPE_COLLECTION = 2,												\
	WORKSHOP_FILE_TYPE_ART = 3,														\
	WORKSHOP_FILE_TYPE_VIDEO = 4,													\
	WORKSHOP_FILE_TYPE_SCREENSHOT = 5,												\
	WORKSHOP_FILE_TYPE_GAME = 6,													\
	WORKSHOP_FILE_TYPE_SOFTWARE = 7,												\
	WORKSHOP_FILE_TYPE_CONCEPT = 8,													\
	WORKSHOP_FILE_TYPE_WEB_GUIDE = 9,												\
    WORKSHOP_FILE_TYPE_INTEGRATED_GUIDE = 10,										\
	WORKSHOP_FILE_TYPE_MERCH = 11,													\
	WORKSHOP_FILE_TYPE_CONTROLLER_BINDING = 12,										\
	WORKSHOP_FILE_TYPE_STEAMWORKS_ACCESS_INVITE = 13,								\
	WORKSHOP_FILE_TYPE_STEAM_VIDEO = 14,											\
	WORKSHOP_FILE_TYPE_GAME_MANAGED_ITEM = 15,										\
	WORKSHOP_FILE_TYPE_MAX = 16														\
};																					\
enum WorkshopVideoProvider {														\
    WORKSHOP_VIDEO_PROVIDER_NONE = 0,												\
	WORKSHOP_VIDEO_PROVIDER_YOUTUBE = 1												\
};																					\
enum WorkshopVote {																	\
    WORKSHOP_VOTE_UNVOTED = 0,														\
	WORKSHOP_VOTE_FOR = 1,															\
	WORKSHOP_VOTE_AGAINST = 2,														\
	WORKSHOP_VOTE_LATER = 3															\
};																					\
enum LocalFileChange {																\
    LOCAL_FILE_CHANGE_INVALID = 0,													\
	LOCAL_FILE_CHANGE_FILE_UPDATED = 1,												\
	LOCAL_FILE_CHANGE_FILE_DELETED = 2												\
};																					\
enum FilePathType {																	\
    FILE_PATH_TYPE_INVALID = 0,														\
	FILE_PATH_TYPE_ABSOLUTE = 1,													\
	FILE_PATH_TYPE_API_FILENAME = 2													\
};																					\
																					\
/* Screenshot enums*/																\
enum VRScreenshotType {																\
    VR_SCREENSHOT_TYPE_NONE = 0,													\
	VR_SCREENSHOT_TYPE_MONO = 1,													\
	VR_SCREENSHOT_TYPE_STEREO = 2,													\
	VR_SCREENSHOT_TYPE_MONO_CUBE_MAP = 3,											\
	VR_SCREENSHOT_TYPE_MONO_PANORAMA = 4,											\
	VR_SCREENSHOT_TYPE_STEREO_PANORAMA = 5											\
};																					\
																					\
/* UGC enums*/																		\
enum ItemPreviewType {																\
    ITEM_PREVIEW_TYPE_IMAGE = 0,													\
	ITEM_PREVIEW_TYPE_YOUTUBE_VIDEO = 1,											\
	ITEM_PREVIEW_TYPE_SKETCHFAB = 2,												\
	ITEM_PREVIEW_TYPE_ENVIRONMENTMAP_HORIZONTAL_CROSS = 3,							\
	ITEM_PREVIEW_TYPE_ENVIRONMENTMAP_LAT_LONG = 4,									\
	ITEM_PREVIEW_TYPE_RESERVED_MAX = 255											\
};																					\
enum ItemState {																	\
    ITEM_STATE_NONE = 0,															\
	ITEM_STATE_SUBSCRIBED = 1,														\
	ITEM_STATE_LEGACY_ITEM = 2,														\
	ITEM_STATE_INSTALLED = 4,														\
	ITEM_STATE_NEEDS_UPDATE = 8,													\
	ITEM_STATE_DOWNLOADING = 16,													\
	ITEM_STATE_DOWNLOAD_PENDING = 32												\
};																					\
enum ItemStatistic {																\
    ITEM_STATISTIC_NUM_SUBSCRIPTIONS = 0,											\
	ITEM_STATISTIC_NUM_FAVORITES = 1,												\
	ITEM_STATISTIC_NUM_FOLLOWERS = 2,												\
	ITEM_STATISTIC_NUM_UNIQUE_SUBSCRIPTIONS = 3,									\
	ITEM_STATISTIC_NUM_UNIQUE_FAVORITES = 4,										\
	ITEM_STATISTIC_NUM_UNIQUE_FOLLOWERS = 5,										\
	ITEM_STATISTIC_NUM_UNIQUE_WEBSITE_VIEWS = 6,									\
	ITEM_STATISTIC_REPORT_SCORE = 7,												\
	ITEM_STATISTIC_NUM_SECONDS_PLAYED = 8,											\
    ITEM_STATISTIC_NUM_PLAYTIME_SESSIONS = 9,										\
	ITEM_STATISTIC_NUM_COMMENTS = 10,												\
	ITEM_STATISTIC_NUM_SECONDS_PLAYED_DURING_TIME_PERIOD = 11,						\
	ITEM_STATISTIC_NUM_PLAYTIME_SESSIONS_DURING_TIME_PERIOD = 12					\
};																					\
enum ItemUpdateStatus {																\
    ITEM_UPDATE_STATUS_INVALID = 0,													\
	ITEM_UPDATE_STATUS_PREPARING_CONFIG = 1,										\
	ITEM_UPDATE_STATUS_PREPARING_CONTENT = 2,										\
	ITEM_UPDATE_STATUS_UPLOADING_CONTENT = 3,										\
	ITEM_UPDATE_STATUS_UPLOADING_PREVIEW_FILE = 4,									\
	ITEM_UPDATE_STATUS_COMMITTING_CHANGES = 5										\
};																					\
enum UGCMatchingUGCType {															\
    UGC_MATCHINGUGCTYPE_ITEMS = 0,													\
	UGC_MATCHING_UGC_TYPE_ITEMS_MTX = 1,											\
	UGC_MATCHING_UGC_TYPE_ITEMS_READY_TO_USE = 2,									\
	UGC_MATCHING_UGC_TYPE_COLLECTIONS = 3,											\
	UGC_MATCHING_UGC_TYPE_ARTWORK = 4,												\
	UGC_MATCHING_UGC_TYPE_VIDEOS = 5,												\
	UGC_MATCHING_UGC_TYPE_SCREENSHOTS = 6,											\
	UGC_MATCHING_UGC_TYPE_ALL_GUIDES = 7,											\
	UGC_MATCHING_UGC_TYPE_WEB_GUIDES = 8,											\
    UGC_MATCHING_UGC_TYPE_INTEGRATED_GUIDES = 9,									\
	UGC_MATCHING_UGC_TYPE_USABLE_IN_GAME = 10,										\
	UGC_MATCHING_UGC_TYPE_CONTROLLER_BINDINGS = 11,									\
	UGC_MATCHING_UGC_TYPE_GAME_MANAGED_ITEMS = 12,									\
	UGC_MATCHING_UGC_TYPE_ALL = ~0													\
};																					\
enum UGCQuery {																		\
    UGC_QUERY_RANKED_BY_VOTE = 0,													\
	UGC_QUERY_RANKED_BY_PUBLICATION_DATE = 1,										\
	UGC_QUERY_ACCEPTED_FOR_GAME_RANKED_BY_ACCEPTANCE_DATE = 2,						\
	UGC_QUERY_RANKED_BY_TREND = 3,													\
	UGC_QUERY_FAVORITED_BY_FRIENDS_RANKED_BY_PUBLICATION_DATE = 4,					\
	UGC_QUERY_CREATED_BY_FRIENDS_RANKED_BY_PUBLICATION_DATE = 5,					\
	UGC_QUERY_RANKED_BY_NUM_TIMES_REPORTED = 6,										\
    UGC_QUERY_CREATED_BY_FOLLOWED_USERS_RANKED_BY_PUBLICATION_DATE = 7,				\
	UGC_QUERY_NOT_YET_RATED = 8,													\
	UGC_QUERY_RANKED_BY_TOTAL_VOTES_ASC = 9,										\
	UGC_QUERY_RANKED_BY_VOTES_UP = 10,												\
	UGC_QUERY_RANKED_BY_TEXT_SEARCH = 11,											\
	UGC_QUERY_RANKED_BY_TOTAL_UNIQUE_SUBSCRIPTIONS = 12,							\
	UGC_QUERY_RANKED_BY_PLAYTIME_TREND = 13,										\
	UGC_QUERY_RANKED_BY_TOTAL_PLAYTIME = 14,										\
    UGC_QUERY_RANKED_BY_AVERAGE_PLAYTIME_TREND = 15,								\
	UGC_QUERY_RANKED_BY_LIFETIME_AVERAGE_PLAYTIME = 16,								\
	UGC_QUERY_RANKED_BY_PLAYTIME_SESSIONS_TREND = 17,								\
	UGC_QUERY_RANKED_BY_LIFETIME_PLAYTIME_SESSIONS = 18,							\
	UGC_QUERY_RANKED_BY_LAST_UPDATED_DATE = 19										\
};																					\
enum UserUGCList {																	\
    USER_UGC_LIST_PUBLISHED = 0,													\
	USER_UGC_LIST_VOTED_ON = 1,														\
	USER_UGC_LIST_VOTED_UP = 2,														\
	USER_UGC_LIST_VOTED_DOWN = 3,													\
	USER_UGC_LIST_FAVORITED = 5,													\
	USER_UGC_LIST_SUBSCRIBED = 6,													\
	USER_UGC_LIST_USED_OR_PLAYED = 7,												\
	USER_UGC_LIST_FOLLOWED = 8														\
};																					\
enum UserUGCListSortOrder {															\
    USERUGCLISTSORTORDER_CREATIONORDERDESC = 0,										\
	USERUGCLISTSORTORDER_CREATIONORDERASC = 1,										\
	USERUGCLISTSORTORDER_TITLEASC = 2,												\
	USERUGCLISTSORTORDER_LASTUPDATEDDESC = 3,										\
	USERUGCLISTSORTORDER_SUBSCRIPTIONDATEDESC = 4,									\
	USERUGCLISTSORTORDER_VOTESCOREDESC = 5,											\
	USERUGCLISTSORTORDER_FORMODERATION = 6											\
};																					\
enum UGCContentDescriptorID {														\
    UGCCONTENTDESCRIPTOR_NUDITY_OR_SEXUAL_CONTENT = 1,								\
	UGCCONTENTDESCRIPTOR_FREQUENT_VIOLENCE_OR_GORE = 2,								\
	UGCCONTENTDESCRIPTOR_ADULT_ONLY_SEXUAL_CONTENT = 3,								\
	UGCCONTENTDESCRIPTOR_GRATUITOUS_SEXUAL_CONTENT = 4,								\
	UGCCONTENTDESCRIPTOR_ANY_MATURE_CONTENT = 5										\
};																					\
																					\
/* User enums*/																		\
enum FailureType {																	\
    FAILURE_FLUSHED_CALLBACK_QUEUE = 0,												\
	FAILURE_PIPE_FAIL = 1															\
};																					\
enum DurationControlProgress {														\
    DURATION_CONTROL_PROGRESS_FULL = 0,												\
	DURATION_CONTROL_PROGRESS_HALF = 1,												\
	DURATION_CONTROL_PROGRESS_NONE = 2												\
};																					\
enum DurationControlNotification {													\
    DURATION_CONTROL_NOTIFICATION_NONE = 0,											\
	DURATION_CONTROL_NOTIFICATION_1_HOUR = 1,										\
	DURATION_CONTROL_NOTIFICATION_3_HOURS = 3,										\
	DURATION_CONTROL_NOTIFICATION_HALF_PROGRESS = 3,								\
	DURATION_CONTROL_NOTIFICATION_NO_PROGRESS = 4									\
};																					\
																					\
/* User Stats enums*/																\
enum LeaderboardDataRequest {														\
    LEADERBOARD_DATA_REQUEST_GLOBAL = 0,											\
	LEADERBOARD_DATA_REQUEST_GLOBAL_AROUND_USER = 1,								\
	LEADERBOARD_DATA_REQUEST_FRIENDS = 2,											\
	LEADERBOARD_DATA_REQUEST_USERS = 3												\
};																					\
enum LeaderboardDisplayType {														\
    LEADERBOARD_DISPLAY_TYPE_NONE = 0,												\
	LEADERBOARD_DISPLAY_TYPE_NUMERIC = 1,											\
	LEADERBOARD_DISPLAY_TYPE_TIME_SECONDS = 2,										\
	LEADERBOARD_DISPLAY_TYPE_TIME_MILLISECONDS = 3									\
};																					\
enum LeaderboardSortMethod {														\
    LEADERBOARD_SORT_METHOD_NONE = 0,												\
	LEADERBOARD_SORT_METHOD_ASCENDING = 1,											\
	LEADERBOARD_SORT_METHOD_DESCENDING = 2											\
};																					\
enum LeaderboardUploadScoreMethod {													\
    LEADERBOARD_UPLOAD_SCORE_METHOD = 0,											\
	LEADERBOARD_UPLOAD_SCORE_METHOD_KEEP_BEST = 1,									\
	LEADERBOARD_UPLOAD_SCORE_METHOD_FORCE_UPDATE = 2								\
};																					\
enum SteamUserStatType {															\
    STEAM_USER_STAT_TYPE_INVALID = 0,												\
	STEAM_USER_STAT_TYPE_INT = 1,													\
	STEAM_USER_STAT_TYPE_FLOAT = 2,													\
	STEAM_USER_STAT_TYPE_AVGRATE = 3,												\
	STEAM_USER_STAT_TYPE_ACHIEVEMENTS = 4,											\
	STEAM_USER_STAT_TYPE_GROUPACHIEVEMENTS = 5,										\
	STEAM_USER_STAT_TYPE_MAX = 6													\
};																					\
																					\
/* Utils enums*/																	\
enum CheckFileSignature {															\
    CHECK_FILE_SIGNATURE_INVALID_SIGNATURE = 0,										\
	CHECK_FILE_SIGNATURE_VALID_SIGNATURE = 1,										\
	CHECK_FILE_SIGNATURE_FILE_NOT_FOUND = 2,										\
	CHECK_FILE_SIGNATURE_NO_SIGNATURES_FOUND_FOR_THIS_APP = 3,						\
	CHECK_FILE_SIGNATURE_NO_SIGNATURES_FOUND_FOR_THIS_FILE = 4						\
};																					\
enum GamepadTextInputLineMode {														\
    GAMEPAD_TEXT_INPUT_LINE_MODE_SINGLE_LINE = 0,									\
	GAMEPAD_TEXT_INPUT_LINE_MODE_MULTIPLE_LINES = 1									\
};																					\
enum GamepadTextInputMode {															\
    GAMEPAD_TEXT_INPUT_MODE_NORMAL = 0,												\
	GAMEPAD_TEXT_INPUT_MODE_PASSWORD = 1											\
};																					\
enum FloatingGamepadTextInputMode {													\
    FLOATING_GAMEPAD_TEXT_INPUT_MODE_SINGLE_LINE = 0,								\
	FLOATING_GAMEPAD_TEXT_INPUT_MODE_MULTIPLE_LINES = 1,							\
	FLOATING_GAMEPAD_TEXT_INPUT_MODE_EMAIL = 2,										\
	FLOATING_GAMEPAD_TEXT_INPUT_MODE_NUMERIC = 3,									\
};																					\
enum SteamAPICallFailure {															\
    STEAM_API_CALL_FAILURE_NONE = -1,												\
	STEAM_API_CALL_FAILURE_STEAM_GONE = 0,											\
	STEAM_API_CALL_FAILURE_NETWORK_FAILURE = 1,										\
	STEAM_API_CALL_FAILURE_INVALID_HANDLE = 2,										\
	STEAM_API_CALL_FAILURE_MISMATCHED_CALLBACK = 3									\
};																					\
enum TextFilteringContext {															\
    TEXT_FILTERING_CONTEXT_UNKNOWN = 0,												\
    TEXT_FILTERING_CONTEXT_GAME_CONTENT = 1,										\
    TEXT_FILTERING_CONTEXT_CHAT = 2,												\
    TEXT_FILTERING_CONTEXT_NAME = 3													\
};
#define STEAM_VARIANT_CASTS()											\
VARIANT_ENUM_CAST(Steam::AccountType);									\
VARIANT_BITFIELD_CAST(Steam::AppOwnershipFlags);						\
VARIANT_ENUM_CAST(Steam::AppReleaseState);								\
VARIANT_BITFIELD_CAST(Steam::AppType);									\
VARIANT_ENUM_CAST(Steam::AuthSessionResponse);							\
VARIANT_ENUM_CAST(Steam::BeginAuthSessionResult);						\
VARIANT_ENUM_CAST(Steam::BroadcastUploadResult);						\
VARIANT_ENUM_CAST(Steam::ChatEntryType);								\
VARIANT_ENUM_CAST(Steam::ChatRoomEnterResponse);						\
VARIANT_BITFIELD_CAST(Steam::ChatSteamIDInstanceFlags);					\
VARIANT_ENUM_CAST(Steam::DenyReason);									\
VARIANT_ENUM_CAST(Steam::GameIDType);									\
VARIANT_ENUM_CAST(Steam::LaunchOptionType);								\
VARIANT_ENUM_CAST(Steam::NotificationPosition);							\
VARIANT_ENUM_CAST(Steam::Result);										\
VARIANT_ENUM_CAST(Steam::Universe);										\
VARIANT_ENUM_CAST(Steam::UserHasLicenseForAppResult);					\
VARIANT_ENUM_CAST(Steam::VoiceResult);									\
VARIANT_ENUM_CAST(Steam::VRHMDType);									\
VARIANT_ENUM_CAST(Steam::AvatarSizes);									\
VARIANT_BITFIELD_CAST(Steam::FriendFlags);								\
VARIANT_ENUM_CAST(Steam::FriendRelationship);							\
VARIANT_ENUM_CAST(Steam::OverlayToStoreFlag);							\
VARIANT_ENUM_CAST(Steam::OverlayToWebPageMode);							\
VARIANT_BITFIELD_CAST(Steam::PersonaChange);							\
VARIANT_ENUM_CAST(Steam::PersonaState);									\
VARIANT_BITFIELD_CAST(Steam::UserRestriction);							\
VARIANT_ENUM_CAST(Steam::GameSearchErrorCode);							\
VARIANT_ENUM_CAST(Steam::PlayerResult);									\
VARIANT_BITFIELD_CAST(Steam::HTMLKeyModifiers);							\
VARIANT_ENUM_CAST(Steam::HTMLMouseButton);								\
VARIANT_ENUM_CAST(Steam::MouseCursor);									\
VARIANT_ENUM_CAST(Steam::HTTPMethod);									\
VARIANT_ENUM_CAST(Steam::HTTPStatusCode);								\
VARIANT_ENUM_CAST(Steam::InputActionOrigin);							\
VARIANT_ENUM_CAST(Steam::SteamInputType);								\
VARIANT_BITFIELD_CAST(Steam::ConfigurationEnableType);					\
VARIANT_ENUM_CAST(Steam::GlyphSize);									\
VARIANT_BITFIELD_CAST(Steam::GlyphStyle);								\
VARIANT_BITFIELD_CAST(Steam::SteamItemFlags);							\
VARIANT_BITFIELD_CAST(Steam::ChatMemberStateChange);					\
VARIANT_ENUM_CAST(Steam::LobbyComparison);								\
VARIANT_ENUM_CAST(Steam::LobbyDistanceFilter);							\
VARIANT_ENUM_CAST(Steam::LobbyType);									\
VARIANT_ENUM_CAST(Steam::MatchMakingServerResponse);					\
VARIANT_ENUM_CAST(Steam::AudioPlaybackStatus);							\
VARIANT_ENUM_CAST(Steam::P2PSend);										\
VARIANT_ENUM_CAST(Steam::P2PSessionError);								\
VARIANT_ENUM_CAST(Steam::SNetSocketConnectionType);						\
VARIANT_ENUM_CAST(Steam::SNetSocketState);								\
VARIANT_ENUM_CAST(Steam::NetworkingConfigValue);						\
VARIANT_ENUM_CAST(Steam::NetworkingGetConfigValueResult);				\
VARIANT_ENUM_CAST(Steam::NetworkingConnectionState);					\
VARIANT_ENUM_CAST(Steam::NetworkingConnectionEnd);						\
VARIANT_ENUM_CAST(Steam::NetworkingIdentityType);						\
VARIANT_ENUM_CAST(Steam::NetworkingSocketsDebugOutputType);				\
VARIANT_ENUM_CAST(Steam::NetworkingAvailability);						\
VARIANT_ENUM_CAST(Steam::NetworkingConfigScope);						\
VARIANT_ENUM_CAST(Steam::NetworkingConfigDataType);						\
VARIANT_ENUM_CAST(Steam::ParentalFeature);								\
VARIANT_ENUM_CAST(Steam::SteamPartyBeaconLocationType);					\
VARIANT_ENUM_CAST(Steam::SteamPartyBeaconLocationData);					\
VARIANT_BITFIELD_CAST(Steam::RemoteStoragePlatform);					\
VARIANT_ENUM_CAST(Steam::RemoteStoragePublishedFileVisibility);			\
VARIANT_ENUM_CAST(Steam::UGCReadAction);								\
VARIANT_ENUM_CAST(Steam::WorkshopEnumerationType);						\
VARIANT_ENUM_CAST(Steam::WorkshopFileAction);							\
VARIANT_ENUM_CAST(Steam::WorkshopFileType);								\
VARIANT_ENUM_CAST(Steam::WorkshopVideoProvider);						\
VARIANT_ENUM_CAST(Steam::WorkshopVote);									\
VARIANT_ENUM_CAST(Steam::LocalFileChange);								\
VARIANT_ENUM_CAST(Steam::FilePathType);									\
VARIANT_ENUM_CAST(Steam::VRScreenshotType);								\
VARIANT_ENUM_CAST(Steam::ItemPreviewType);								\
VARIANT_BITFIELD_CAST(Steam::ItemState);								\
VARIANT_ENUM_CAST(Steam::ItemStatistic);								\
VARIANT_ENUM_CAST(Steam::ItemUpdateStatus);								\
VARIANT_ENUM_CAST(Steam::UGCMatchingUGCType);							\
VARIANT_ENUM_CAST(Steam::UGCQuery);										\
VARIANT_ENUM_CAST(Steam::UserUGCList);									\
VARIANT_ENUM_CAST(Steam::UserUGCListSortOrder);							\
VARIANT_ENUM_CAST(Steam::UGCContentDescriptorID);						\
VARIANT_ENUM_CAST(Steam::FailureType);									\
VARIANT_ENUM_CAST(Steam::DurationControlProgress);						\
VARIANT_ENUM_CAST(Steam::DurationControlNotification);					\
VARIANT_ENUM_CAST(Steam::LeaderboardDataRequest);						\
VARIANT_ENUM_CAST(Steam::LeaderboardDisplayType);						\
VARIANT_ENUM_CAST(Steam::LeaderboardSortMethod);						\
VARIANT_ENUM_CAST(Steam::LeaderboardUploadScoreMethod);					\
VARIANT_ENUM_CAST(Steam::SteamUserStatType);							\
VARIANT_ENUM_CAST(Steam::CheckFileSignature);							\
VARIANT_ENUM_CAST(Steam::GamepadTextInputLineMode);						\
VARIANT_ENUM_CAST(Steam::GamepadTextInputMode);							\
VARIANT_ENUM_CAST(Steam::FloatingGamepadTextInputMode);					\
VARIANT_ENUM_CAST(Steam::SteamAPICallFailure);							\
VARIANT_ENUM_CAST(Steam::TextFilteringContext);							\
VARIANT_ENUM_CAST(Steam::CommunityProfileItemType);						\
VARIANT_ENUM_CAST(Steam::CommunityProfileItemProperty);					\
VARIANT_ENUM_CAST(Steam::SCEPadTriggerEffectMode);