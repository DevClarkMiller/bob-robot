export const swapAndFetchToken = (newTokenKeyName: string) => {
	const newToken = localStorage.getItem(newTokenKeyName) as string;
	const oldToken = localStorage.getItem('token') ?? '';
	localStorage.setItem('token', newToken);

	return oldToken;
};
