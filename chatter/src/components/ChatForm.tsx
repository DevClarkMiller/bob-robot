import { useForm } from 'react-hook-form';
import { zodResolver } from '@hookform/resolvers/zod';
import { z } from 'zod';
import { Form, FormControl, FormField, FormItem, FormLabel, FormMessage } from './ui/form';
import { Button } from './ui/button';
import { Input } from './ui/input';
import { useEffect } from 'react';
import { Card } from './ui/card';

import { poster } from 'helios-utilities-sdk';
import { BaseApiUrl } from '@/Constants';

const ChatForm = () => {
	const formSchema = z.object({
		unitGuid: z.string().regex(/^[0-9a-f]{8}-[0-9a-f]{4}-[0-9a-f]{4}-[0-9a-f]{4}-[0-9a-f]{12}$/i, {
			message: 'UnitGuid must be an actual Guid',
		}),
		message: z
			.string()
			.min(1, { message: 'Message must be at least one character' })
			.max(128, { message: 'Message has a max of 128 characters' })
			.nonempty(),
	});

	const form = useForm<z.infer<typeof formSchema>>({
		resolver: zodResolver(formSchema),
		defaultValues: {
			unitGuid: '',
			message: '',
		},
	});

	const onSubmit = async (values: z.infer<typeof formSchema>) => {
		localStorage.setItem('unitGuid', values.unitGuid);

		const route = `${BaseApiUrl}/Chat`;
		const url = new URL(route);

		url.searchParams.append('unitGuid', values.unitGuid);
		url.searchParams.append('message', values.message);

		poster(url, null);
		form.resetField('message');
	};

	useEffect(() => {
		const unitGuid = localStorage.getItem('unitGuid');
		if (unitGuid) form.setValue('unitGuid', unitGuid);
	}, []);

	return (
		<Card className="p-4">
			<Form {...form}>
				<form onSubmit={form.handleSubmit(onSubmit)} className="space-y-8">
					<FormField
						control={form.control}
						name="unitGuid"
						render={({ field }) => (
							<FormItem>
								<FormLabel>Unit Guid</FormLabel>
								<FormControl>
									<Input placeholder="87487969-0636-4196-9e1a-718f6c80e626" {...field} />
								</FormControl>
								<FormMessage />
							</FormItem>
						)}
					/>
					<FormField
						control={form.control}
						name="message"
						render={({ field }) => (
							<FormItem>
								<FormLabel>Message</FormLabel>
								<FormControl>
									<Input placeholder="Heyyy, wyd??? Wanna ft??" {...field} />
								</FormControl>
								<FormMessage />
							</FormItem>
						)}
					/>
					<Button type="submit">Submit</Button>
				</form>
			</Form>
		</Card>
	);
};

export default ChatForm;
