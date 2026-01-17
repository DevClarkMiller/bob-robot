import { useEffect, useState } from 'react';
import useStoredField from './useStoredField';
import { fetcher, putter } from 'helios-utilities-sdk';
import { BaseApiUrl } from '@/Constants';

const useSettings = () => {
	const { value: unitGuid, setValue: setUnitGuid } = useStoredField('unitGuid');

	const [isUsingAIChats, setIsUsingAIChats] = useState(false);

	useEffect(() => {
		(async () => {
			if (!unitGuid) return;

			const url = new URL(`${BaseApiUrl}/Chat/isUsingAIChats`);
			url.searchParams.append('unitGuid', unitGuid);

			const resp = await fetcher(url, true);
			console.log(resp.data);
			if (resp.data) setIsUsingAIChats(resp.data == 'true');
		})();
	}, [unitGuid]);

	const onChangeIsUsingAIChats = async (state: boolean) => {
		const url = new URL(`${BaseApiUrl}/Chat/isUsingAIChats`);
		url.searchParams.append('unitGuid', unitGuid);
		url.searchParams.append('state', state ? 'true' : 'false');

		await putter(url, true);
		setIsUsingAIChats(state);
	};

	return { isUsingAIChats, unitGuid, setUnitGuid, onChangeIsUsingAIChats };
};

export default useSettings;
