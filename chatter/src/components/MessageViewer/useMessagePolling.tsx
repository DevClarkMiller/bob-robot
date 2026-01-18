import { BaseApiUrl, POLL_INTERVAL_MS } from '@/Constants';
import { swapAndFetchToken } from '@/helpers/TokenHelper';
import type { MessageType } from '@/models/MessageType';
import { fetcher } from 'helios-utilities-sdk';
import { useCallback, useEffect, useState } from 'react';

const useMessagePolling = (unitGuid: string): MessageType[] => {
	const [messages, setMessages] = useState<MessageType[]>([]);

	const pollMessage = useCallback(async () => {
		const url = new URL(`${BaseApiUrl}/Chat`);
		url.searchParams.append('unitGuid', unitGuid);

		// FIX FETCHER
		try {
			console.log(url.toString());

			const oldToken = swapAndFetchToken('chatterToken');
			const resp = await fetcher<string>(url, true);
			localStorage.setItem('token', oldToken);
			if (!resp || resp.error || !resp.data) return;

			if (resp.data == 'No chats queued up') return;

			setMessages(msgsPrev => [...msgsPrev, { text: resp.data as string, timeReceived: new Date() }]);
		} catch (err) {
			console.log(err);
		}
	}, [unitGuid]);

	useEffect(() => {
		const interval = setInterval(() => {
			pollMessage();
		}, POLL_INTERVAL_MS);

		return () => clearInterval(interval);
	}, [pollMessage, unitGuid]);

	return messages;
};

export default useMessagePolling;
