import './App.css';
import { Route, Routes, useNavigate, useSearchParams } from 'react-router-dom';

import React, { useCallback, useEffect, useMemo } from 'react';
import ChatForm from './components/ChatForm';
import MessageViewer from './components/MessageViewer/MessageViewer';
import SettingsInput from './components/SettingsInput';
import useSettings from './hooks/useSettings';
import UnitKeyboard from './components/UnitKeyboard';
import NotFoundPage from './components/NotFoundPage';

interface HomeProps {
	isLoading: boolean;
	isUsingAIChats: boolean;
	unitGuid: string;
	setUnitGuid: React.Dispatch<React.SetStateAction<string>>;
	onChangeIsUsingAIChats: (state: boolean) => Promise<void>;
}

const Home = ({ isLoading, isUsingAIChats, unitGuid, setUnitGuid, onChangeIsUsingAIChats }: HomeProps) => {
	return (
		<>
			<h1 className="font-bold text-lg mb-5">Bot the Robot Chatter</h1>
			<div className="w-100 mx-auto">
				<SettingsInput
					isLoading={isLoading}
					isUsingAIChats={isUsingAIChats}
					unitGuid={unitGuid}
					setUnitGuid={setUnitGuid}
					onChangeIsUsingAIChats={onChangeIsUsingAIChats}
				/>
			</div>
			<div className="w-100 mx-auto">
				<MessageViewer unitGuid={unitGuid} />
			</div>
			<div className="w-100 mx-auto">
				<ChatForm unitGuid={unitGuid} />
			</div>
		</>
	);
};

function App() {
	const navigate = useNavigate();
	const [searchParams] = useSearchParams();

	const { isLoading, isUsingAIChats, unitGuid, setUnitGuid, onChangeIsUsingAIChats } = useSettings();
	const paramChatterToken = useMemo(() => searchParams.get('chatterToken'), [searchParams]);
	const paramUnitToken = useMemo(() => searchParams.get('unitToken'), [searchParams]);
	const paramUnitGuid = useMemo(() => searchParams.get('unitGuid'), [searchParams]);

	const storeAndRemoveParam = useCallback(
		(param: any, keyName: string) => {
			if (!param) return;

			localStorage.setItem(keyName, param);
			const params = new URLSearchParams(searchParams);
			params.delete(keyName);
			navigate({ search: params.toString() }, { replace: true });
		},
		[navigate, searchParams]
	);

	useEffect(() => {
		storeAndRemoveParam(paramChatterToken, 'chatterToken');
		storeAndRemoveParam(paramUnitToken, 'unitToken');
		storeAndRemoveParam(paramUnitGuid, 'unitGuid');
	}, [navigate, paramChatterToken, paramUnitGuid, paramUnitToken, searchParams, storeAndRemoveParam]);

	// TODO: ADD TOGGLE TO MAKE CHATGPT RESPOND TO CHATS INSTEAD WHEN I DONT FEEL LIKE IT
	// so add a new controller, add route to toggle chat gpt, when chats come in, check if chatgpt
	// is toggled, if so respond right away, else add to queue

	return (
		<div className="h-screen w-screen flex flex-col justify-center gap-2 text-center">
			<Routes>
				<Route path="/keyboard" element={<UnitKeyboard unitGuid={unitGuid} />} />
				<Route
					path="/"
					element={
						<Home
							isLoading={isLoading}
							isUsingAIChats={isUsingAIChats}
							unitGuid={unitGuid}
							setUnitGuid={setUnitGuid}
							onChangeIsUsingAIChats={onChangeIsUsingAIChats}
						/>
					}
				/>
				<Route path="*" element={<NotFoundPage />} />
			</Routes>
		</div>
	);
}

export default App;
