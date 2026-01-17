import { POLL_INTERVAL_MS } from '@/Constants';
import { Card, CardContent, CardDescription, CardHeader, CardTitle } from '../ui/card';
import Message from './Message';
import useMessagePolling from './useMessagePolling';

export interface MessageViewerProps {
	unitGuid: string;
}

const MessageViewer = ({ unitGuid }: MessageViewerProps) => {
	const messages = useMessagePolling(unitGuid);

	return (
		<Card className="w-100">
			<CardHeader className="text-start">
				<CardTitle>Messages</CardTitle>
				<CardDescription>Polling new messages on {POLL_INTERVAL_MS / 1000} second interval</CardDescription>
			</CardHeader>
			<CardContent className="max-x-100 text-h-36 max-h-36 overflow-y-scroll overflow-x-clip">
				{messages.length > 0 ? (
					<div className="text-start break-words">
						{messages.map((msg, idx) => (
							<Message message={msg} key={`MSG-${idx}`} />
						))}
					</div>
				) : (
					<>No Messages...</>
				)}
			</CardContent>
		</Card>
	);
};

export default MessageViewer;
