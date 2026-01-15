import './App.css';
import { useSearchParams } from 'react-router-dom';

import { Button } from './components/ui/button';
import { useEffect, useMemo } from 'react';
import ChatForm from './components/ChatForm';

function App() {
	const [searchParams] = useSearchParams();

	const paramToken = useMemo(() => searchParams.get('token'), [searchParams]);

	useEffect(() => {
		if (!paramToken) return;

		localStorage.setItem('token', paramToken);
	}, [paramToken]);

	return (
		<>
			<ChatForm />
		</>
	);
}

export default App;
