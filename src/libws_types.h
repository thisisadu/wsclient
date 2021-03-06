
#ifndef __LIBWS_TYPES_H__
#define __LIBWS_TYPES_H__

#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>

#ifdef _WIN32
#include <time.h>
#else
#include <sys/time.h>
#endif

typedef struct ws_s *ws_t;
typedef struct ws_base_s *ws_base_t;

enum{
  WS_CONNECT_FAILED = -2,
  WS_CONNECT_BREAK = -1
};

typedef enum ws_opcode_e
{
	WS_OPCODE_CONTINUATION_0X0 = 0x0,
	WS_OPCODE_TEXT_0X1 = 0x1,
	WS_OPCODE_BINARY_0X2 = 0x2,

	// 0x3 - 0x7 Reserved for further non-control frames.
	WS_OPCODE_NON_CONTROL_RSV_0X3 = 0x3,
	WS_OPCODE_NON_CONTROL_RSV_0X4 = 0x4,
	WS_OPCODE_NON_CONTROL_RSV_0X5 = 0x5,
	WS_OPCODE_NON_CONTROL_RSV_0X6 = 0x6,
	WS_OPCODE_NON_CONTROL_RSV_0X7 = 0x7,

	WS_OPCODE_CLOSE_0X8 = 0x8,
	WS_OPCODE_PING_0X9 = 0x9,
	WS_OPCODE_PONG_0XA = 0xA,

	// 0xB - 0xF are reserved for further control frames.
	WS_OPCODE_CONTROL_RSV_0XB = 0xB,
	WS_OPCODE_CONTROL_RSV_0XC = 0xC,
	WS_OPCODE_CONTROL_RSV_0XD = 0xD,
	WS_OPCODE_CONTROL_RSV_0XE = 0xE,
	WS_OPCODE_CONTROL_RSV_0XF = 0xF
} ws_opcode_t;

typedef enum ws_close_status_e
{
	/// 1000 indicates a normal closure, meaning that the purpose for
    /// which the connection was established has been fulfilled.
	WS_CLOSE_STATUS_NORMAL_1000 = 1000,

	/// 1001 indicates that an endpoint is "going away", such as a server
    /// going down or a browser having navigated away from a page.
	WS_CLOSE_STATUS_GOING_AWAY_1001 = 1001,

	/// 1002 indicates that an endpoint is terminating the connection due
    /// to a protocol error.
	WS_CLOSE_STATUS_PROTOCOL_ERR_1002 = 1002,

	/// 1003 indicates that an endpoint is terminating the connection
    /// because it has received a type of data it cannot accept (e.g., an
    /// endpoint that understands only text data MAY send this if it
    /// receives a binary message).
	WS_CLOSE_STATUS_UNSUPPORTED_DATA_1003 = 1003,

	/// Reserved.  The specific meaning might be defined in the future.
	WS_CLOSE_STATUS_RESERVED_1004 = 1004,

	/// 1005 is a reserved value and MUST NOT be set as a status code in a
	/// Close control frame by an endpoint.  It is designated for use in
	/// applications expecting a status code to indicate that no status
	/// code was actually present.
	WS_CLOSE_STATUS_STATUS_CODE_EXPECTED_1005 = 1005,

	/// 1006 is a reserved value and MUST NOT be set as a status code in a
	/// Close control frame by an endpoint.  It is designated for use in
	/// applications expecting a status code to indicate that the
	/// connection was closed abnormally, e.g., without sending or
	/// receiving a Close control frame.
	WS_CLOSE_STATUS_ABNORMAL_1006 = 1006,

	/// 1007 indicates that an endpoint is terminating the connection
	/// because it has received data within a message that was not
	/// consistent with the type of the message (e.g., non-UTF-8 [RFC3629]
	/// data within a text message).
	WS_CLOSE_STATUS_INCONSISTENT_DATA_1007 = 1007,

	/// 1008 indicates that an endpoint is terminating the connection
	/// because it has received a message that violates its policy.  This
	/// is a generic status code that can be returned when there is no
	/// other more suitable status code (e.g., 1003 or 1009) or if there
	/// is a need to hide specific details about the policy.
	WS_CLOSE_STATUS_POLICY_VIOLATION_1008 = 1008,

	/// 1009 indicates that an endpoint is terminating the connection
	/// because it has received a message that is too big for it to
	/// process.
	WS_CLOSE_STATUS_MESSAGE_TOO_BIG_1009 = 1009,

	/// 1010 indicates that an endpoint (client) is terminating the
	/// connection because it has expected the server to negotiate one or
	/// more extension, but the server didn't return them in the response
	/// message of the WebSocket handshake.  The list of extensions that
	/// are needed SHOULD appear in the /reason/ part of the Close frame.
	/// Note that this status code is not used by the server, because it
	/// can fail the WebSocket handshake instead.
	WS_CLOSE_STATUS_EXTENSION_NOT_NEGOTIATED_1010 = 1010,

	/// 1011 indicates that a server is terminating the connection because
	/// it encountered an unexpected condition that prevented it from
	/// fulfilling the request.
	WS_CLOSE_STATUS_UNEXPECTED_CONDITION_1011 = 1011,

	/// 1015 is a reserved value and MUST NOT be set as a status code in a
	/// Close control frame by an endpoint.  It is designated for use in
	/// applications expecting a status code to indicate that the
	/// connection was closed due to a failure to perform a TLS handshake
	/// (e.g., the server certificate can't be verified).
	WS_CLOSE_STATUS_FAILED_TLS_HANDSHAKE_1015 = 1015
} ws_close_status_t;

#define WS_IS_CLOSE_STATUS_NOT_USED(code) \
	(((int)code < 1000) || ((int)code > 4999))

/// Status codes in the range 1000-2999 are reserved for definition by
/// this protocol, its future revisions, and extensions specified in a
/// permanent and readily available public specification.
#define WS_IS_CLOSE_STATUS_WEBSOCKET_RESERVED(code) \
	(((int)code >= 1000) && ((int)code <= 2999))

/// Status codes in the range 3000-3999 are reserved for use by
/// libraries, frameworks, and applications.  These status codes are
/// registered directly with IANA.  The interpretation of these codes
/// is undefined by this protocol.
#define WS_IS_CLOSE_STATUS_IANA_RESERVED(code) \
	(((int)code >= 3000) && ((int)code <= 3999))

/// Is the close code meant to only be used locally by a websocket
/// endpoint. That is, if it's not allowed to be sent to a peer as
/// a close code.
#define WS_IS_CLOSE_STATUS_LOCAL(code) \
	(  (code == WS_CLOSE_STATUS_RESERVED_1004) \
	|| (code == WS_CLOSE_STATUS_STATUS_CODE_EXPECTED_1005) \
	|| (code == WS_CLOSE_STATUS_ABNORMAL_1006) \
	|| (code == WS_CLOSE_STATUS_FAILED_TLS_HANDSHAKE_1015))

/// Status codes in the range 4000-4999 are reserved for private use
/// and thus can't be registered.  Such codes can be used by prior
/// agreements between WebSocket applications.  The interpretation of
/// these codes is undefined by this protocol.
#define WS_IS_CLOSE_STATUS_PRIVATE_USE(code) \
	(((int)code >= 4000) && ((int)code <= 4999))

#define WS_OPCODE_IS_CONTROL(opcode) \
	(((int)opcode >= WS_OPCODE_CLOSE_0X8) \
	&& ((int)opcode <= WS_OPCODE_CONTROL_RSV_0XF))

#define WS_OPCODE_IS_RESERVED_CONTROL(opcode) \
	(((int)opcode >= WS_OPCODE_CONTROL_RSV_0XB) \
	&& ((int)opcode <= WS_OPCODE_CONTROL_RSV_0XF))

#define WS_OPCODE_IS_RESERVED_NON_CONTROL(opcode) \
	(((int)opcode >= WS_OPCODE_NON_CONTROL_RSV_0X3) \
	&& ((int)opcode <= WS_OPCODE_NON_CONTROL_RSV_0X7))

#define WS_OPCODE_IS_RESERVED(opcode) \
	(WS_OPCODE_IS_RESERVED_CONTROL(opcode) \
	|| WS_OPCODE_IS_RESERVED_NON_CONTROL(opcode))

/// Has the peer sent a valid close code?
#define WS_IS_PEER_CLOSE_STATUS_VALID(code) \
	(WS_IS_CLOSE_STATUS_IANA_RESERVED(code) \
	|| (WS_IS_CLOSE_STATUS_PRIVATE_USE(code)) \
	|| (code == WS_CLOSE_STATUS_NORMAL_1000) \
	|| (code == WS_CLOSE_STATUS_GOING_AWAY_1001) \
	|| (code == WS_CLOSE_STATUS_PROTOCOL_ERR_1002) \
	|| (code == WS_CLOSE_STATUS_UNSUPPORTED_DATA_1003) \
	|| (code == WS_CLOSE_STATUS_INCONSISTENT_DATA_1007) \
	|| (code == WS_CLOSE_STATUS_POLICY_VIOLATION_1008) \
	|| (code == WS_CLOSE_STATUS_MESSAGE_TOO_BIG_1009) \
	|| (code == WS_CLOSE_STATUS_EXTENSION_NOT_NEGOTIATED_1010) \
	|| (code == WS_CLOSE_STATUS_UNEXPECTED_CONDITION_1011))

#define WS_MAX_PAYLOAD_LEN 0x7FFFFFFFFFFFFFFF
#define WS_CONTROL_MAX_PAYLOAD_LEN 125

#define WS_HDR_BASE_SIZE 2
#define WS_HDR_PAYLOAD_LEN_SIZE 8
#define WS_HDR_MASK_SIZE 4
#define WS_HDR_MIN_SIZE WS_HDR_BASE_SIZE
#define WS_HDR_MAX_SIZE (WS_HDR_BASE_SIZE \
						+ WS_HDR_PAYLOAD_LEN_SIZE \
						+ WS_HDR_MASK_SIZE)

///
/// Websocket header structure.
///
/// 
/// Websocket protocol RFC 6455: http://tools.ietf.org/html/rfc6455
///
///      0                   1                   2                   3
///      0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
///     +-+-+-+-+-------+-+-------------+-------------------------------+
///     |F|R|R|R| opcode|M| Payload len |    Extended payload length    |
///     |I|S|S|S|  (4)  |A|     (7)     |             (16/64)           |
///     |N|V|V|V|       |S|             |   (if payload len==126/127)   |
///     | |1|2|3|       |K|             |                               |
///     +-+-+-+-+-------+-+-------------+ - - - - - - - - - - - - - - - +
///     |     Extended payload length continued, if payload len == 127  |
///     + - - - - - - - - - - - - - - - +-------------------------------+
///     |                               |Masking-key, if MASK set to 1  |
///     +-------------------------------+-------------------------------+
///     | Masking-key (continued)       |          Payload Data         |
///     +-------------------------------- - - - - - - - - - - - - - - - +
///     :                     Payload Data continued ...                :
///     + - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - +
///     |                     Payload Data continued ...                |
///     +---------------------------------------------------------------+
///
typedef struct ws_header_s
{
	uint8_t fin;			///< Final frame bit.
	uint8_t rsv1;			///< Reserved bit 1 for extensions.
	uint8_t rsv2;			///< Reserved bit 2 for extensions. 
	uint8_t rsv3;			///< Reserved bit 3 for extensions.
	ws_opcode_t opcode;		///< Operation code.
	uint8_t mask_bit;		///< If this frame is masked this bit is set.
	uint64_t payload_len;	///< Length of the payload.
	uint32_t mask;			///< Masking key for the payload.
} ws_header_t;

#define WS_MAX_FRAME_SIZE 0x7FFFFFFFFFFFFFFF
#define WS_DEFAULT_CONNECT_TIMEOUT 60

typedef enum ws_state_e
{
	WS_STATE_DNS_LOOKUP,
	WS_STATE_CLOSING,
	WS_STATE_CLOSING_UNCLEANLY,
	WS_STATE_CLOSED_CLEANLY,
	WS_STATE_CLOSED_UNCLEANLY,
	WS_STATE_CONNECTING,
	WS_STATE_CONNECTED
} ws_state_t;

typedef enum ws_parse_state_e
{
	WS_PARSE_STATE_USER_ABORT = -2,
	WS_PARSE_STATE_ERROR = -1,
	WS_PARSE_STATE_SUCCESS = 0,
	WS_PARSE_STATE_NEED_MORE
} ws_parse_state_t;

#ifdef LIBWS_WITH_OPENSSL
typedef enum libws_ssl_state_e
{
	LIBWS_SSL_OFF,
	LIBWS_SSL_ON,
	LIBWS_SSL_SELFSIGNED
} libws_ssl_state_t;
#endif // LIBWS_WITH_OPENSSL

#define WS_RANDOM_PATH "/dev/urandom"

typedef void (*ws_msg_callback_f)(ws_t ws, char *msg, uint64_t len,
			int binary, void *arg);

typedef void (*ws_msg_begin_callback_f)(ws_t ws, void *arg);
typedef void (*ws_msg_frame_callback_f)(ws_t ws, char *payload, 
										uint64_t len, void *arg);
typedef void (*ws_msg_end_callback_f)(ws_t ws, void *arg);

typedef void (*ws_msg_frame_begin_callback_f)(ws_t ws, void *arg);
typedef void (*ws_msg_frame_data_callback_f)(ws_t ws, char *payload,
											uint64_t len, void *arg);
typedef void (*ws_msg_frame_end_callback_f)(ws_t ws, void *arg);

typedef void (*ws_err_callback_f)(ws_t ws, int errcode,
								const char *errmsg, void *arg);
typedef void (*ws_close_callback_f)(ws_t ws, ws_close_status_t status,
				const char *reason, size_t reason_len, void *arg);
typedef void (*ws_connect_callback_f)(ws_t ws, void *arg);
typedef void (*ws_timeout_callback_f)(ws_t ws,
				struct timeval timeout, void *arg);
typedef void (*ws_no_copy_cleanup_f)(ws_t ws, const void *data,
						uint64_t datalen, void *extra);
typedef int (*ws_header_callback_f)(ws_t ws, const char *header_name,
				const char *header_val, void *arg);

typedef void *(*ws_malloc_replacement_f)(size_t bytes);
typedef void (*ws_free_replacement_f)(void *ptr);
typedef void *(*ws_realloc_replacement_f)(void *ptr, size_t bytes);

#endif // __LIBWS_TYPES_H__
