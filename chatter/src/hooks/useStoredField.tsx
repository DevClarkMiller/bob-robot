import { useEffect, useState } from 'react';

const useStoredField = (keyName: string) => {
	const [value, setValue] = useState('');

	useEffect(() => {
		const storedVal = localStorage.getItem(keyName);
		if (storedVal) setValue(storedVal);
	}, [keyName]);

	useEffect(() => {
		localStorage.setItem(keyName, value);
	}, [keyName, value]);

	return { value, setValue };
};

export default useStoredField;
