import type { MessageType } from '@/models/MessageType';

export interface MessageProps {
	message: MessageType;
}

const Message = ({ message }: MessageProps) => {
	return <div>{message.text}</div>;
};

export default Message;
