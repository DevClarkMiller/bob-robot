import './App.css'
import { useSearchParams } from 'react-router-dom';

import { Button } from './components/ui/button'
import { useEffect, useMemo } from 'react';

function App() {
	const [searchParams] = useSearchParams();

	const paramToken = useMemo(() => searchParams.get('token'), [searchParams]);

	useEffect(() => {
		if (!paramToken) return;

		localStorage.setItem("token", paramToken);
	}, [paramToken]);
	
	return (
		<>
			<Button>Test</Button>
		</>
	)
}

export default App
